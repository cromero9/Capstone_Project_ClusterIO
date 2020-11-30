#include "Cluster_Board.h"

Cluster_Board::Cluster_Board() {}

/**
	* cross-platform function to create threads
	* @param start_routine This is the function pointer for the thread to run
	* @return 0 on success, otherwise an integer above 1
	*/
int Cluster_Board::create_task(void* start_routine)
{
#ifdef WIN32
	thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, NULL, 0, NULL);
	if (thread1 == NULL)
	{
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
int Cluster_Board::init_mutex()
{
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
void Cluster_Board::lock_mutex()
{
#ifdef WIN32
	EnterCriticalSection(&lock);
#else
	pthread_mutex_lock(&lock);
#endif
}

/**
* cross platform mutex unlock
*/
void Cluster_Board::unlock_mutex()
{
#ifdef WIN32
	LeaveCriticalSection(&lock);
#else
	pthread_mutex_unlock(&lock);
#endif
}

/**
* cross-platform sleep
*/
void Cluster_Board::sleep_ms(int milliseconds)
{
#ifdef WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}