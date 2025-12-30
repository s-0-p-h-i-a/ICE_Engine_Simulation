/*
RPM Calculation System:
Hall Sensor Module

Function / Role:
- Read Hall sensor on each loop iteration
- Ignores every second edge to account for backward servo sweep
- Provides delta for RPM calculation

Process:
- Read Hall sensor every loop
- On magnet detection:
	- If forward sweep (valid read): update timestamps
	- Toggle secondPass to skip next detection
- Compute delta time, feed it to RPM calculation	
	
System layers (top -> bottom):
flywheel_ref_lib.cpp
servo_lib.cpp
hall_lib.cpp
rpm_lib.cpp

*/

#include "drive.h"
#include "hall_lib.h"
#include <Arduino.h>

//	========= CONSTANT =========
#define ZERO 0

// 	========= VARIABLES =========
// Hall system state
static bool hallState = 0;
static bool lastHallState = 0;
static bool secondPass = 0;

// Timestamps
static long unsigned int lastEventMillis = ZERO;
static long unsigned int previousEventMillis = ZERO;
static long unsigned int currentMillis = ZERO;

//	========= FUNCTIONS =========
// Accessors for plotter display
bool getHallState(void) { return hallState; }
bool getSecondPass(void) { return secondPass; }

// Read sensor and update state
void updateHallState(void) {
	
	// Update timer + state variables
	currentMillis = millis();
	lastHallState = hallState;
	hallState = !digitalRead(HALL_PIN);
	
	// On rising edge
	if (hallState && !lastHallState) {
		if (!secondPass) {
		
			previousEventMillis	= lastEventMillis;
			lastEventMillis		= currentMillis;
		}
		// Sweep direction toggle
		secondPass = !secondPass;
	}
}

// Delta time between valid Hall events (for RPM calculation)
unsigned long int getTimeDelta(void) {
	return currentMillis - previousEventMillis;
}
