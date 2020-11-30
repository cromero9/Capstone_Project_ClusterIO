#pragma once
#include "Cluster_Board.h"

class Blinker : public Cluster_Board
{
public:
	Blinker();
	void* Send_Blinker_Signal_To_Hardware(void* arg);

};
