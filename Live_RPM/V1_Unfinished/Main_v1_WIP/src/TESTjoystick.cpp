/*
Joystick Input Reading Module

Function / Role:
- Joystick input interface
- Allows drive module to update engine state: on/off and speed level

Process:
- Take raw input from joystick X (up/down) and Z (button click)
- Debounce via checkDebounce
- Return "true" joystick input, feed to drive module via accessor functions

Module reading this data:
drive.cpp

*/

#include "joystick.h"
#include <arduino.h>

//	========= CONSTANTS =========
// Debounce time period to ignore jitter
#define DEBOUNCE_DELAY 100
// Joystick pins
#define JOYSTICK_Z 3
static const int JOYSTICK_X = A5;

// 	========= VARIABLES =========
// Joystick X: Old value reference + debounce timestamp
static unsigned int oldX = 0;
static unsigned long lastUpdate_X = 0;

// Joystick Z: Old value reference + debounce timestamp
static bool oldZ = 0;
static unsigned long lastUpdate_Z = 0;

//	========= FUNCTIONS =========
// Helper function
static bool checkDebounce(unsigned long timeNow, unsigned long lastInputTimestamp);

// Initialise joystick
void initJoystick(void) {
	pinMode(JOYSTICK_Z,INPUT);
}

// Checks if debounce period is elapsed
static bool checkDebounce(unsigned long timeNow, unsigned long lastInputTimestamp) {
	
	return timeNow - lastInputTimestamp > DEBOUNCE_DELAY;	
}

// Read and debounce joystick X input (up/down)
unsigned int readJoystick_X(void) {

	unsigned long currentMillis = millis();

	unsigned int newX = oldX;
	
	if (checkDebounce(currentMillis, lastUpdate_X)) {
		lastUpdate_X = currentMillis;
		newX = analogRead(JOYSTICK_X);
		oldX = newX;
	}
	return newX;
}

// Read and debounce joystick Z input (button)
bool readJoystick_Z(void) {

	unsigned long currentMillis = millis();

	bool newZ = oldZ;
	
	if (checkDebounce(currentMillis, lastUpdate_Z)) {
		lastUpdate_Z = currentMillis;
		newZ = digitalRead(JOYSTICK_Z);
		oldZ = newZ;
	}
	return newZ;
}
