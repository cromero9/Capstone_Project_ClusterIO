#pragma once

#include "Cluster_Board.h"




class Send_Receive_Channel : public Cluster_Board
{
public:
	Send_Receive_Channel();
	void* send_thread();
	void* receive_thread();
	


};