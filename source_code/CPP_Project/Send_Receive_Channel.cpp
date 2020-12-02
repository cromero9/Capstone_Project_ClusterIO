#include "Send_Receive_Channel.h"
#include "Fuel_Gauge.h"
#include "Radio_Play.h"


Send_Receive_Channel::Send_Receive_Channel() {}

void* Send_Receive_Channel::receive_thread()
{
	gre_io_t* handle;
	gre_io_serialized_data_t* nbuffer = NULL;
	char* event_addr;
	char* event_name;
	char* event_format;
	void* event_data;
	int						 ret;
	int nbytes;

	Cluster_Board cluster;
	
	Radio_Play radio;

	printf("Opening a channel for receive\n");
	// Connect to a channel to receive messages
	handle = gre_io_open("cluster_backend", GRE_IO_TYPE_RDONLY);
	if (handle == NULL) {
		fprintf(stderr, "Can't open receive channel\n");
		return 0;
	}

	nbuffer = gre_io_size_buffer(NULL, 100);

	while (1) {
		ret = gre_io_receive(handle, &nbuffer);
		if (ret < 0) {
			return 0;
		}

		event_name = NULL;
		nbytes = gre_io_unserialize(nbuffer, &event_addr, &event_name, &event_format, &event_data);
		if (!event_name) {
			printf("Missing event name\n");
			return 0;
		}

		printf("Received Event %s nbytes: %d format: %s\n", event_name, nbytes, event_format);

		cluster.lock_mutex();

		if (strcmp(event_name, "drive_selection") == 0) {
			drive_selection_event_t* uidata = (drive_selection_event_t*)event_data;

			cluster_state.drive = cluster_state.drive + uidata->drive;
			dataChanged = 1;
		}
		else if (strcmp(event_name, "park_selection") == 0) {
			park_selection_event_t* uidata = (park_selection_event_t*)event_data;

			cluster_state.drive = cluster_state.drive - uidata->park;
			dataChanged = 1;
		}

		
		radio.radio_station(event_name, event_data);



		cluster.unlock_mutex();
	}

	//Release the buffer memory, close the send handle
	gre_io_free_buffer(nbuffer);
	gre_io_close(handle);

}


	void* Send_Receive_Channel::send_thread()
	{
		gre_io_t	*send_handle;
		gre_io_serialized_data_t	*nbuffer = NULL;
		cluster_update_event_t 		event_data;
		void* data;
		int 						ret;
		
		Cluster_Board cluster;
		Fuel_Gauge fuel;

		memset(&cluster_state, 0, sizeof(cluster_state));

		cluster_state.drive = 0;

		if (cluster.init_mutex() != 0) {
			fprintf(stderr, "Mutex init failed\n");
			return 0;
		}
		// Connect to a channel to send messages (write)
		printf("Trying to open the connection to the frontend\n");

		while (1)
		{

			// Connect to a channel to send messages (write)
			cluster.sleep_ms(SNOOZE_TIME);
			send_handle = gre_io_open(CLUSTER_SEND_CHANNEL, GRE_IO_TYPE_WRONLY);
			if (send_handle != NULL)
			{
				printf("Send channel: %s successfully opened\n", CLUSTER_SEND_CHANNEL);
				break;
			}

		}

		printf("Starting System\n");
		;
		if (create_task(receive_thread) != 0) {
			//fprintf(stderr, "Thread create failed\n");
			printf("Create Thread Failed");
			return 0;
		}

		printf("here");
		
		
		memset(&event_data, 0, sizeof(event_data));

		//event_data.fuel = 100;


		while (1) {
			lock_mutex();
			// Simulate data acquisition ...
			//memset(&event_data, 0, sizeof(event_data));
			
			if (cluster_state.drive == 1) {
				sleep_ms(80);
				event_data.speed = (event_data.speed + 1) % 200;
				event_data.rpm = (event_data.rpm + 50) % 10000;
				//std::cout << event_data.rpm << "\n";

				

			}
			else {
				sleep_ms(80);
				event_data.speed = 0;
				event_data.rpm = 0;
				//do nothin car is parked
			}
			fuel.check_fuel_level(event_data.rpm);
			
			unlock_mutex();

			

			// Serialize the data to a buffer
			nbuffer = gre_io_serialize(nbuffer, NULL, CLUSTER_UPDATE_EVENT, CLUSTER_UPDATE_FMT, &event_data, sizeof(event_data));
			if (!nbuffer) {
				fprintf(stderr, "Can't serialized data to buffer, exiting\n");
				break;
			}

			// Send the serialized event buffer
			ret = gre_io_send(send_handle, nbuffer);
			if (ret < 0) {
				fprintf(stderr, "Send failed, exiting\n");
				break;
			}
		}

		//Release the buffer memory, close the send handle
		gre_io_free_buffer(nbuffer);
		gre_io_close(send_handle);

		return 0;
	}



