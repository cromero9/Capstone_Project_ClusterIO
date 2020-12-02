#pragma once

#include "Cluster_Board.h" 

class Fuel_Gauge : public Cluster_Board
{
public:
	Fuel_Gauge();
	void* check_fuel_level(uint16_t rpm);

};