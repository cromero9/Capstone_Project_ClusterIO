#include "Receive_Channel.h"
using namespace std;

Receive_Channel::Receive_Channel() {}

void* Receive_Channel::receive_thread(void* arg)
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

		cluster.unlock_mutex();
	}

	//Release the buffer memory, close the send handle
	gre_io_free_buffer(nbuffer);
	gre_io_close(handle);

}



	
