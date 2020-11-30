#include <string.h>
#define CLUSTER_UPDATE_EVENT "cluster_update"
#define CLUSTER_UPDATE_FMT "2u1 speed 2u1 rpm 2u1 fuel 2u1 battery 2u1 oil 2u1 odometer 2u1 trip 2u1 drive 2u1 blinker_left 2u1 blinker_right 2u1 ignition"
typedef struct {
	uint16_t 		speed;
	uint16_t 		rpm;
	uint16_t 		fuel;
	uint16_t 		battery;
	uint16_t 		oil;
	uint16_t 		odometer;
	uint16_t 		trip;
	uint16_t 		drive;
	uint16_t		blinker_left;
	uint16_t		blinker_right;
	uint16_t		ignition;
	

} cluster_update_event_t;

#define DRIVE_SELECTION_EVENT "drive_selection"
#define DRIVE_SELECTION_FMT "2u1 drive"

typedef struct {
	uint16_t drive;

}drive_selection_event_t;

#define PARK_SELECTION_EVENT "park_selection"
#define PARK_SELECTION_FMT "2u1 park"

typedef struct {
	uint16_t park;

}park_selection_event_t;

#define UPDATE_RADIO_EVENT "update_radio"
#define UPDATE_RADIO_FMT "2u1 radio_channel"

typedef struct {
	uint16_t radio_channel;

}update_radio_event_t;

#define BLINKER_LEFT_EVENT "blinker_left"
#define BLINKER_LEFT_FMT "2u1 blinker_left"

typedef struct {
	uint16_t blinker_left;

}blinker_left_event_t;

#define BLINKER_RIGHT_EVENT "blinker_right"
#define BLINKER_RIGHT_FMT "2u1 blinker_right"

typedef struct {
	uint16_t blinker_right;

}blinker_right_event_t;

#define IGNITION_EVENT "ignition"
#define IGNITION_FMT "2u1 ignition"

typedef struct {
	uint16_t ignition;

}ignition_event_t;