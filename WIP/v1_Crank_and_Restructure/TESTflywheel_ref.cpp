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


#include "servo_lib.h"
#include "flywheel_ref.h"

//	========= CONSTANTS =========
#define ANGLE_ZERO 0

// 	========= VARIABLES =========
// Current "flywheel" position
static unsigned int flywheelAngle = ANGLE_ZERO;

//	========= FUNCTIONS =========
// Accessor for plotter display
unsigned int getFlywheelAngle(void) { return flywheelAngle; }

// Stepwise movement via user-defined speed input
void advanceFlywheelAngle(unsigned int crankshaftAngleInput) {
	
	flywheelAngle = crankshaftAngleInput;
	
	moveServo(flywheelAngle);
}
