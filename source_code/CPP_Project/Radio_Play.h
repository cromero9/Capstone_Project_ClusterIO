#pragma once

#include "Cluster_Board.h"

static update_radio_event_t	radio;


class Radio_Play : public Cluster_Board
{
public:
	Radio_Play();
	void* radio_station(char* event_name, void* event_data);

};