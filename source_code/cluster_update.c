
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h> // for usleep
#include <linux/gpio.h>
#include <sys/ioctl.h>
#endif

#include <gre/greio.h>
#include "ClusterIO_events.h"


#define SIMULATION_MAX_TEMP 35
#define SIMULATION_MIN_TEMP 8
#define SNOOZE_TIME 80

static int							dataChanged = 1; //Default to 1 so we send data to the ui once it connects
static cluster_update_event_t	cluster_state;
static ignition_event_t	ignition_state;
#ifdef WIN32
static CRITICAL_SECTION lock;
static HANDLE thread1;
#else 
static pthread_mutex_t lock;
static pthread_t 	thread1;
#endif
/**
 * cross-platform function to create threads
 * @param start_routine This is the function pointer for the thread to run
 * @return 0 on success, otherwise an integer above 1
 */
int
create_task(void* start_routine) {
#ifdef WIN32
	thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, NULL, 0, NULL);
	if (thread1 == NULL) {
		return 1;
	}
	return 0;
#else
	return pthread_create(&thread1, NULL, start_routine, NULL);
#endif
}

/**
 * cross platform mutex initialization
 * @return 0 on success, otherwise an integer above 1
 */
int
init_mutex() {
#ifdef WIN32
	InitializeCriticalSection(&lock);
	return 0;
#else
	return pthread_mutex_init(&lock, NULL);
#endif
}

/**
 * cross platform mutex lock
 */
void
lock_mutex() {
#ifdef WIN32
	EnterCriticalSection(&lock);
#else
	pthread_mutex_lock(&lock);
#endif
}

/**
 * cross platform mutex unlock
 */
void
unlock_mutex() {
#ifdef WIN32
	LeaveCriticalSection(&lock);
#else
	pthread_mutex_unlock(&lock);
#endif
}

/**
 * cross-platform sleep
 */
void
sleep_ms(int milliseconds) {
#ifdef WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}

void*
receive_thread(void* arg) {
	gre_io_t* handle;
	gre_io_serialized_data_t* nbuffer = NULL;
	char* event_addr;
	char* event_name;
	char* event_format;
	void* event_data;
	int						 ret;
	int nbytes;

	//GPIO handles
	struct gpiohandle_request req;
	struct gpiohandle_data data;
	char chrdev_name[20];
	int fd, ret2;

	printf("TEST TEST TEST Opening a channel for receive\n");
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

		lock_mutex();
		if (strcmp(event_name, "drive_selection") == 0) {
			drive_selection_event_t* uidata = (drive_selection_event_t*)event_data;

			cluster_state.drive = 1;
			dataChanged = 1;

		}
		else if (strcmp(event_name, "park_selection") == 0) {
			park_selection_event_t* uidata = (park_selection_event_t*)event_data;

			cluster_state.drive = 0;
			dataChanged = 1;
		}

		//Left Blinker GPIO
		else if (strcmp(event_name, "blinker_left") == 0) {
			blinker_left_event_t* uidata = (blinker_left_event_t*)event_data;
			cluster_state.blinker_left = uidata->blinker_left;

			printf("Left Blinker pressed");
			strcpy(chrdev_name, "/dev/gpiochip6");
			/*  Open device: gpiochip6 for GPIO bank G */
			fd = open(chrdev_name, 0);
			if (fd == -1) {
				ret2 = -errno;
				fprintf(stderr, "Failed to open %s\n", chrdev_name);

				//return ret;
			}

			/* request GPIO line: GPIO_G_3 */
			req.lineoffsets[0] = 3;
			req.flags = GPIOHANDLE_REQUEST_OUTPUT;
			memcpy(req.default_values, &data, sizeof(req.default_values));
			strcpy(req.consumer_label, "led_gpio_g_3");
			req.lines = 1;

			ret2 = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
			if (ret2 == -1) {
				ret2 = -errno;
				fprintf(stderr, "Failed to issue GET LINEHANDLE IOCTL (%d)\n",
					ret2);
			}
			if (close(fd) == -1)
				perror("Failed to close GPIO character device file");

			/*  Start left led blinking */
			if (cluster_state.blinker_left == 1) {

				//data.values[0] = !data.values[0];
				data.values[0] = 1;
				ret2 = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
				if (ret2 == -1) {
					ret2 = -errno;
					fprintf(stderr, "Failed to issue %s (%d)\n",
						"GPIOHANDLE_SET_LINE_VALUES_IOCTL", ret);
				}
				sleep(1); //this was for the while loop
			}
			else if (cluster_state.blinker_left == 0) {
				data.values[0] = 0;
				ret2 = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
				if (ret2 == -1) {
					ret2 = -errno;
					fprintf(stderr, "Failed to issue %s (%d)\n",
						"GPIOHANDLE_SET_LINE_VALUES_IOCTL", ret);
				}
				//sleep(1); //this was for the while loop


			}

			/*  release line */
			ret2 = close(req.fd);
			if (ret2 == -1) {
				perror("Failed to close GPIO LINEHANDLE device file");
				ret2 = -errno;
			}


		}

		//Right Blinker GPIO
		else if (strcmp(event_name, "blinker_right") == 0) {
			blinker_right_event_t* uidata = (blinker_right_event_t*)event_data;
			cluster_state.blinker_right = uidata->blinker_right;

			printf("Right Blinker pressed");
			strcpy(chrdev_name, "/dev/gpiochip3");
			/*  Open device: gpiochip6 for GPIO bank D */
			fd = open(chrdev_name, 0);
			if (fd == -1) {
				ret2 = -errno;
				fprintf(stderr, "Failed to open %s\n", chrdev_name);

				//return ret;
			}

			/* request GPIO line: GPIO_D_1 */
			req.lineoffsets[0] = 1;
			req.flags = GPIOHANDLE_REQUEST_OUTPUT;
			memcpy(req.default_values, &data, sizeof(req.default_values));
			strcpy(req.consumer_label, "led_gpio_d_1");
			req.lines = 1;

			ret2 = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
			if (ret2 == -1) {
				ret2 = -errno;
				fprintf(stderr, "Failed to issue GET LINEHANDLE IOCTL (%d)\n",
					ret2);
			}
			if (close(fd) == -1)
				perror("Failed to close GPIO character device file");

			/*  Start left led blinking */
			if (cluster_state.blinker_right == 1) {

				//data.values[0] = !data.values[0];
				data.values[0] = 1;
				ret2 = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
				if (ret2 == -1) {
					ret2 = -errno;
					fprintf(stderr, "Failed to issue %s (%d)\n",
						"GPIOHANDLE_SET_LINE_VALUES_IOCTL", ret);
				}
				sleep(1); //this was for the while loop
			}
			else if (cluster_state.blinker_right == 0) {
				data.values[0] = 0;
				ret2 = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
				if (ret2 == -1) {
					ret2 = -errno;
					fprintf(stderr, "Failed to issue %s (%d)\n",
						"GPIOHANDLE_SET_LINE_VALUES_IOCTL", ret);
				}
				//sleep(1); //this was for the while loop


			}

			/*  release line */
			ret2 = close(req.fd);
			if (ret2 == -1) {
				perror("Failed to close GPIO LINEHANDLE device file");
				ret2 = -errno;
			}


		}

		unlock_mutex();
	}

	//Release the buffer memory, close the send handle
	gre_io_free_buffer(nbuffer);
	gre_io_close(handle);

}

int
main(int argc, char** argv) {
	gre_io_t* send_handle;
	gre_io_serialized_data_t* nbuffer = NULL;
	cluster_update_event_t 		event_data;
	ignition_event_t	ignition_event;
	int 						ret;

	//GPIO handles
	struct gpiohandle_request req;
	struct gpiohandle_data data;
	char chrdev_name[20];
	int fd, ret2;

	memset(&cluster_state, 0, sizeof(cluster_state));
	memset(&ignition_state, 0, sizeof(ignition_state));

	cluster_state.drive = 0;

	if (init_mutex() != 0) {
		fprintf(stderr, "Mutex init failed\n");
		return 0;
	}
	// Connect to a channel to send messages (write)
	printf("Trying to open the connection to the frontend\n");

	while (1) {
		// Connect to a channel to send messages (write)
		sleep_ms(SNOOZE_TIME);
		send_handle = gre_io_open("cluster", GRE_IO_TYPE_WRONLY);
		if (send_handle != NULL) {
			printf("Send channel: cluster successfully opened\n");
			break;
		}
	}

	if (create_task(receive_thread) != 0) {
		fprintf(stderr, "Thread create failed\n");
		return 0;
	}


	memset(&event_data, 0, sizeof(event_data));




	while (1) {
		//Read ignition pin
		strcpy(chrdev_name, "/dev/gpiochip4");
		/*  Open device: gpiochip4 for GPIO bank E */
		fd = open(chrdev_name, 0);
		if (fd == -1) {
			ret2 = -errno;
			fprintf(stderr, "Failed to open %s\n", chrdev_name);

			return ret2;
		}

		/* request GPIO line: GPIO_E_1 */
		req.lineoffsets[0] = 1;
		req.flags = GPIOHANDLE_REQUEST_INPUT;
		memcpy(req.default_values, &data, sizeof(req.default_values));
		strcpy(req.consumer_label, "led_gpio_E_1");
		req.lines = 1;

		ret2 = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
		if (ret2 == -1) {
			ret2 = -errno;
			fprintf(stderr, "Failed to issue GET LINEHANDLE IOCTL (%d)\n",
				ret2);
		}

		memset(&data, 0, sizeof(data));

		ret2 = ioctl(req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
		if (ret2 == -1) {
			ret2 = -errno;
			fprintf(stderr, "Failed to issue GPIOHANDLE_GET_LINE_VALUES_IOCTL (%d)\n",
				ret2);
		}


		if (close(fd) == -1)
			perror("Failed to close GPIO character device file");
		

		
		//printf("ret2 %d", ret2);
		//printf("gpio value %d", data.values[0]);
		

		if (data.values[0] == 1) {
			// Simulate data acquisition ...
			//printf("ret2 %d", ret2);
			event_data.ignition = 1;
			if (cluster_state.drive == 1) {
				sleep_ms(80);
				event_data.speed = (event_data.speed + 1) % 200;
				event_data.rpm = (event_data.rpm + 50) % 10000;
				int drive1 = cluster_state.drive;
				//std::cout << drive1;
			}
			else {
				sleep_ms(80);
				event_data.speed = 0;
				event_data.rpm = 0;
				//printf("ret2 %d", ret2);

			}
		}
		else {
			event_data.ignition = 0;
			event_data.speed = 0;
			event_data.rpm = 0;
		}
		

		/*  release line */
		ret2 = close(req.fd);
		if (ret2 == -1) {
			perror("Failed to close GPIO LINEHANDLE device file");
			ret2 = -errno;
		}

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
