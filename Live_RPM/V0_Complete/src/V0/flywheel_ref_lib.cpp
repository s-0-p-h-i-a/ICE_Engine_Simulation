/*
"Flywheel" Reference Generator Module for RPM Calculation System

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

#include "flywheel_lib.h"
#include "servolib.h"
#include <stdint.h>

#define ZERO 0

enum {
	ANGLE_ZERO 	= 0,
	ANGLE_180 	= 180
};

// Initialise system
static uint8_t sweepBack = 0;
int flywheelAngle = ZERO;
int flywheelSpeed = ZERO;

// Helper functions
static void handleEdgeValues(void);
static uint8_t checkDirectionToggle(void);

// Prevent angle from going beyond 0-180
static void handleEdgeValues(void) {
	
	if (flywheelAngle < ANGLE_ZERO) { 
		flywheelAngle = ANGLE_ZERO;
		sweepBack = 1; }
			
	if (flywheelAngle > ANGLE_180) {
		flywheelAngle = ANGLE_180;
		sweepBack = 1; }	
}

// If edge angles reached: switch to opposite "speed" (angle increment/decrement) 
static uint8_t checkDirectionToggle(void) {
	
	uint8_t toggleSpeedZero = (flywheelAngle == ANGLE_ZERO) && (flywheelSpeed < ZERO);
	uint8_t toggleSpeed180  = (flywheelAngle == ANGLE_180)  && (flywheelSpeed > ZERO);
			
	uint8_t toggleNow = toggleSpeedZero || toggleSpeed180;
	sweepBack = toggleNow;
	
	return toggleNow;
}

// Stepwise movement via user-defined speed input
void spinFlywheel(int speed) {
	
	handleEdgeValues();
	
	checkDirectionToggle();
	
	if (sweepBack) { flywheelSpeed = -flywheelSpeed; }
	
	flywheelSpeed = (flywheelSpeed >= ZERO) ? speed : -speed;
	
	flywheelAngle += flywheelSpeed;
	
	moveServo(flywheelAngle);
}
