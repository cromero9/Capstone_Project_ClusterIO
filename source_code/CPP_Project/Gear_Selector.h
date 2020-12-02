#pragma once

#include "Cluster_Board.h"


class Gear_Selector : public Cluster_Board
{
public:
	Gear_Selector();
	void* activate_transmission(char* event_name, void* event_data);

};