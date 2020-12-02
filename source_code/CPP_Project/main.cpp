#include "Cluster_Board.h"
#include "Gear_Selector.h"
#include "Radio_Play.h"
#include "Fuel_Gauge.h"
#include "Send_Receive_Channel.h"
#include "Blinker.h"


int main(int argc, char** argv)
{
	//START APP WITH send_thread here:

	Send_Receive_Channel start_app;
	start_app.send_thread();


	return 0;

}
