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
#include "cylinderVisuals.h"
#include "flywheel_lib.h"
#include "drive.h"
#include <stdint.h>
#include <arduino.h>

//	========= CONSTANTS =========
// Wait period to avoid state oscillation during button press
#define BUTTON_WAIT 700

// Joystick input processing to avoid analog jitter
typedef enum { hyst = 50, IDLE_INPUT = 500 } IdleInputRange;

// Flywheel angle step levels
typedef enum { FLYWHEEL_STEP_IDLE = 2, FLYWHEEL_STEP_ONE = 4, FLYWHEEL_STEP_TWO	= 8 } FlywheelStep;

// Cylinder LED display timing levels
typedef enum { SPEED_TWO_PAUSE = 100, SPEED_ONE_PAUSE = 550, IDLE_PAUSE = 1000 } CylinderDisplayPause;

// Idle input range
static const unsigned int IDLE_RANGE_CEILING = IDLE_INPUT + hyst;
static const unsigned int IDLE_RANGE_FLOOR = IDLE_INPUT - hyst;	

// 	========= VARIABLES =========
// Engine states
bool isIdle 	 = 0;
bool engineON = 0;
// Speed levels, mutually exclusive
bool speedOneInput = 0;
bool speedTwoInput = 0;
// Joystick button debounce timestamp
static unsigned long lastSwitch = 0;

//	========= FUNCTIONS =========
// Accessor functions for plotter display
bool getEngineON(void) 		{ return engineON; }
bool getIsIdle(void) 		{ return isIdle; }
bool getSpeedOneInput(void) { return speedOneInput; }
bool getSpeedTwoInput(void) { return speedTwoInput; }

// ENGINE STATE UPDATES
// Turns engine on/off when joystick pressed
void checkEngineON(void) {

	int now = millis();
	
	bool z = readJoystick_Z();
	bool toggleNow = z && (now - lastSwitch > BUTTON_WAIT);
	
	if (toggleNow) {
		lastSwitch = now;
		engineON = !engineON;
	}
}

// Update idle status
void checkIdle(void) {
	
	const int x = readJoystick_X();
	isIdle = engineON && (x < IDLE_RANGE_CEILING) && (x > IDLE_RANGE_FLOOR);
}

// Set speed level
void checkSpeedInput(void) {
	
	int x = readJoystick_X();
	
	speedOneInput = x > IDLE_RANGE_CEILING;
	speedTwoInput = x < IDLE_RANGE_FLOOR;
}

// Full engine system update
void checkEngineState(void) {
	checkEngineON();
	checkIdle();
	checkSpeedInput();
}

// MODULES CONTROL: FLYWHEEL + CYLINDER
// Set angle step increment for flywheel angle
unsigned int assignFlywheelAngleStep(void) {
	
	if (isIdle)				{ return FLYWHEEL_STEP_IDLE; }
	else if (speedOneInput) { return FLYWHEEL_STEP_ONE; }
	else if (speedTwoInput) { return FLYWHEEL_STEP_TWO; }
}

// Set cylinder LED display blink pause
unsigned int assignCylinderBlinkPause(void) {
	
	if (isIdle)				{ return IDLE_PAUSE; }
	else if (speedOneInput) { return SPEED_ONE_PAUSE; }
	else if (speedTwoInput) { return SPEED_TWO_PAUSE; }
}

// MAIN DRIVE FUNCTION
// Full state update and control value assignment
void driveEngine(void) {
	
	checkEngineState();
	
	if (engineON) {
		
		advanceFlywheelAngle(assignFlywheelAngleStep()); // controls "flywheel" circular movement
		
		displayCylinder(assignCylinderBlinkPause());	 // controls cylinder LED display
	}
}
