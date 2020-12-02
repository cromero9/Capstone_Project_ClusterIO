#include "Radio_Play.h"

Radio_Play::Radio_Play() {}

void* Radio_Play::radio_station(char* event_name, void* event_data)
{
	
	if (strcmp(event_name, "update_radio") == 0) {
		update_radio_event_t* uidata = (update_radio_event_t*)event_data;

		radio.radio_channel = uidata->radio_channel;

		dataChanged = 1;
		std::cout << "radio selected: " << radio.radio_channel << "\n";
	}

	return 0;
}