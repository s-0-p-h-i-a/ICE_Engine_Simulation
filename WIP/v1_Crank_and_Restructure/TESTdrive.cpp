/*
System-wide Control Module

Function / Role:
- Process joystick input:
	- Z for on/off
	- X for speed levels
- Control sub-system modules according to user input

Process:
- Update "engine" system state every loop iteration
- Assign specific speed control values:
	- Flywheel: speed level -> step angle
	- Cylinder: speed level -> LED blink pause
- Send speed values to sub-systems

Control flow: Joystick -> Engine State -> Flywheel + Cylinder

Modules controlled by drive:
flywheel_ref_lib.cpp
cylinder_visuals.cpp

*/

#include "joystick.h"
#include "drive.h"
#include <arduino.h>

//	========= CONSTANTS =========
// Wait period to avoid state oscillation during button press
#define BUTTON_WAIT 700

// Joystick X input processing to avoid analog jitter
typedef enum { hyst = 50, IDLE_INPUT = 500 } IdleInputRange;
// Throttle opening levels (user input)
typedef enum { IDLE = 0, THROTTLE_LEVEL_ONE = 1, THROTTLE_LEVEL_TWO = 2 } ThrottleLevels;

// Idle input range
static const unsigned int IDLE_RANGE_CEILING = IDLE_INPUT + hyst;
static const unsigned int IDLE_RANGE_FLOOR = IDLE_INPUT - hyst;	

// 	========= VARIABLES =========
// Throttle input states
static bool noThrottleInput = 0;
static bool throttleLevelOne = 0;
static bool throttleLevelTwo = 0;
// Joystick button debounce timestamp
static unsigned long lastSwitch = 0;

//	========= FUNCTIONS =========
// Accessor functions for plotter display
bool getNoThrottleInput(void) 	{ return noThrottleInput; }
bool getThrottleLevelOne(void)  { return throttleLevelOne; }
bool getThrottleLevelTwo(void)	{ return throttleLevelTwo; }

// DRIVER INPUT READINF
// Turn engine on/off when joystick pressed
bool turnEngineOnOff(bool engineState) {

	int now = millis();
	
	bool z = readJoystick_Z();
	bool toggleNow = z && (now - lastSwitch > BUTTON_WAIT);
	
	if (toggleNow) {
		lastSwitch = now;
		engineState = !engineState;
	}
	return engineState;
}

// Read throttle opening and assign level (joystick neutral/up/down)
unsigned int assignThrottleLevel(void) {
	
	const int x = readJoystick_X();
	
	noThrottleInput  = engineON && (x < IDLE_RANGE_CEILING) && (x > IDLE_RANGE_FLOOR);
	throttleLevelOne = x > IDLE_RANGE_CEILING;
	throttleLevelTwo = x < IDLE_RANGE_FLOOR;
	
	if (noThrottleInput) {
		return IDLE;
	} else if (throttleLevelOne) {
		return THROTTLE_LEVEL_ONE;
	} else if (throttleLevelTwo) {
		return THROTTLE_LEVEL_TWO;
	}
}
