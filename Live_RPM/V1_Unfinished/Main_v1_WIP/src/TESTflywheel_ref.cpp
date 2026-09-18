/*
RPM Calculation System:
"Flywheel" Reference Generator Module

Function / Role:
- Reference generator layer producing a rotating angular signal using a servo + magnet for live RPM calculation
- Servo module mirrors angles above 180° to produce backward sweep motion
- Hall sensor module accounts for reverse sweep by ignoring alternate edges

Design Rationale / Constraints:
- Magnet mounted on servo conceptually mirrors the reference edge of a 60-2 crank wheel
- Single magnet mounted on "flywheel" disc easier to build + doesn't overload mini servo

System layers (top -> bottom):
flywheel_ref_lib.cpp
servo_lib.cpp
hall_lib.cpp
rpm_lib.cpp

*/

#include "flywheel_lib_ref.h"
#include "servo_lib.h"
#include <stdint.h>

//	========= CONSTANTS =========
enum {
	ANGLE_ZERO 	= 0,
	ANGLE_360 	= 360
};

// 	========= VARIABLES =========
// Current "flywheel" position
static unsigned int flywheelAngle = ANGLE_ZERO;

//	========= FUNCTIONS =========
// Helper function
static void wrapFlywheelAngle(void);

// Accessor for plotter display
unsigned int getFlywheelAngle(void) { return flywheelAngle; }

// Wraps flywheelAngle into [0°, 360°) domain
static void wrapFlywheelAngle(void) {
		
	if (flywheelAngle > ANGLE_360) {
		flywheelAngle = flywheelAngle % ANGLE_360;
	}	
}

// Stepwise movement via user-defined speed input
void advanceFlywheelAngle(unsigned int flywheelAngleStep) {
	
	flywheelAngle += flywheelAngleStep;
	wrapFlywheelAngle();
	
	moveServo(flywheelAngle);
}
