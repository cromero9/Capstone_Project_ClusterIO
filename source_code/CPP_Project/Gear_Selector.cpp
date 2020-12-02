#include "Gear_Selector.h"

Gear_Selector::Gear_Selector(){}

void* Gear_Selector::activate_transmission(char* event_name, void* event_data)
{
	if (strcmp(event_name, "drive_selection") == 0) {
		drive_selection_event_t* uidata = (drive_selection_event_t*)event_data;
		
		cluster_state.drive = cluster_state.drive + uidata->drive;

		dataChanged = 1;
		std::cout << "drive selected " << cluster_state.drive << "\n";
	}
	else if (strcmp(event_name, "park_selection") == 0) {
		park_selection_event_t* uidata = (park_selection_event_t*)event_data;

		cluster_state.drive = cluster_state.drive - uidata->park;
		dataChanged = 1;
		std::cout << "park selected " << cluster_state.drive << "\n";
	}

	return 0;
}