#pragma once
#include "Cluster_Board.h"


class Receive_Channel : public Cluster_Board
{
public:
	Receive_Channel();
	void* receive_thread(void* arg);
	
	

};