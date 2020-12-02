#include "Gear_Selector.h"
#include "Fuel_Gauge.h"

Fuel_Gauge::Fuel_Gauge() {}

void* Fuel_Gauge::check_fuel_level(uint16_t rpm)
{
	
	
	//std::cout << rpm << "\n";
	if (rpm == 1000) {
		if (cluster_state.fuel != 100) {
			cluster_state.fuel = 100;
			//printf("down fuel");
			dataChanged = 1;
			std::cout << cluster_state.fuel << " 0 rpm ";
		}
	}

	else if (rpm == 2000)
	{
		if (cluster_state.fuel != 90) {
			cluster_state.fuel = 90;
			dataChanged = 1;
			std::cout << cluster_state.fuel << " 1000 rpm ";
		}
	}
	else if (rpm == 3000)
	{
		if (cluster_state.fuel != 80) {
			cluster_state.fuel = 80;
			dataChanged = 1;
			std::cout << cluster_state.fuel << " 2000 rpm ";
		}
	}
	else if (rpm == 4000)
	{
		if (cluster_state.fuel != 70) {
			cluster_state.fuel = 70;
			dataChanged = 1;

		}
	}
	else if (rpm == 5000)
	{
		if (cluster_state.fuel != 60) {
			cluster_state.fuel =60;
			dataChanged = 1;
		}
	}
	else if (rpm == 6000)
	{
		if (cluster_state.fuel != 50) {
			cluster_state.fuel = 50;
			dataChanged = 1;
		}
	}
	else if (rpm == 7000)
	{
		if (cluster_state.fuel != 40) {
			cluster_state.fuel = 40;
			dataChanged = 1;
		}
	}
	else if (rpm == 8000)
	{
		if (cluster_state.fuel != 30) {
			cluster_state.fuel = 30;
			dataChanged = 1;
		}
	}
	else if (rpm == 9000)
	{
		if (cluster_state.fuel != 20) {
			cluster_state.fuel = 20;
			dataChanged = 1;
		}
	}
	else if (rpm == 9500)
	{
		if (cluster_state.fuel != 10) {
			cluster_state.fuel = 10;
			dataChanged = 1;
		}
	}
	else if (rpm == 10000)
	{
		if (cluster_state.fuel != 0) {
			cluster_state.fuel = 0;
			cluster_state.drive = cluster_state.drive - 1;
			dataChanged = 1;
		}
	}
	

	return 0;
}