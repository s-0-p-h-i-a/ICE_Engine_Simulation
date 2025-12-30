/*
RPM Calculation System:
"Flywheel" Reference Generator Module

Function / Role:
- Actuator layer controlling a standard servo + magnet used to trigger a Hall sensor for live RPM calculation
- Hall sensor module handles servo sweep back motion by ignoring every other signal detection event starting from the 2nd one

Design Rationale / Constraints:
- Magnet mounted on servo conceptually mirrors the reference edge of a 60-2 crank wheel
- Single magnet mounted on "flywheel" disc easier to build + doesn't overload mini servo

Rest of the system:
servolib.cpp
hall_lib.cpp
rpmlib.cpp

*/

#include "flywheel_lib_ref.h"
#include "servolib.h"
#include <stdint.h>

//	========= CONSTANTS =========
#define ZERO 0

enum {
	ANGLE_ZERO 	= 0,
	ANGLE_180 	= 180
};

// 	========= VARIABLES / Initialise system =========
// Set when angle limits are reached; flips flywheelSpeed sign
static uint8_t shouldReverseDirection = 0;

// Current "flywheel" position
int flywheelAngle = ZERO;
// Current step angle increment/decrement
int flywheelAngleStep = ZERO;

//	========= FUNCTIONS =========
// Helper functions
static void clampAngleAndDetectLimits(void);
static uint8_t updateDirection(void);

// Clamps flywheelAngle to [ANGLE_ZERO, ANGLE_180]
// Sets direction when an angle limit is reached
static void clampAngleAndDetectLimits(void) {
	
	if (flywheelAngle < ANGLE_ZERO) { 
		flywheelAngle = ANGLE_ZERO;
	}
			
	if (flywheelAngle > ANGLE_180) {
		flywheelAngle = ANGLE_180;
	}	
}

// If edge angles reached: switch to opposite "speed" (angle increment/decrement) 
static uint8_t updateDirection(void) {
	
	uint8_t reverseAtZero = (flywheelAngle == ANGLE_ZERO) && (flywheelAngleStep < ZERO);
	uint8_t reverseAt180  = (flywheelAngle == ANGLE_180)  && (flywheelAngleStep > ZERO);
			
	uint8_t reverseDirection = reverseAtZero || reverseAt180;
	shouldReverseDirection = reverseDirection;
	
	return reverseDirection;
}

// Stepwise movement via user-defined speed input
void advanceFlywheelAngle(int step) {
	
	clampAngleAndDetectLimits();
	
	updateDirection();
	
	if (shouldReverseDirection) { flywheelAngleStep = -flywheelAngleStep; }
	
	flywheelAngleStep = (flywheelAngleStep >= ZERO) ? step : -step;
	
	flywheelAngle += flywheelAngleStep;
	
	moveServo(flywheelAngle);
}
