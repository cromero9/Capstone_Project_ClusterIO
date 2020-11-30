#pragma once
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <sys/ioctl.h>
//#include <unistd.h>
//#include <linux/gpio.h>
#include <inttypes.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h> // for usleep
#endif

#include <gre/greio.h>
#include "ClusterIO_events.h"
#include <iostream>




#define SIMULATION_MAX_TEMP 35
#define SIMULATION_MIN_TEMP 8
#define SNOOZE_TIME 80

#define CLUSTER_SEND_CHANNEL "cluster"
#define CLUSTER_RECEIVE_CHANNEL "cluster_backend"

static int							dataChanged = 1; //Default to 1 so we send data to the ui once it connects
static cluster_update_event_t	cluster_state;
#ifdef WIN32
static CRITICAL_SECTION lock;
static HANDLE thread1;
#else 
static pthread_mutex_t lock;
static pthread_t 	thread1;
#endif

class Cluster_Board
{
	public:
		Cluster_Board();
		int create_task(void* start_routine);
		int init_mutex();
		void lock_mutex();
		void unlock_mutex();
		void sleep_ms(int milliseconds);

};



