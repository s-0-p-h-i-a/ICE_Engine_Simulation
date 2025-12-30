#include "joystick.h"
#include <arduino.h>

#define ZERO 0

static const int DEBOUNCE_DELAY	= 100;
static const int ANALOG_CORRECTION = 128;
static const int BUTTON_DELAY 		= 1000;

static long unsigned lastUpdate_X = ZERO;
static long unsigned lastUpdate_Z = ZERO;

int x = ZERO;
bool z = false;

int readJoystick_X(void) {

	unsigned long thisTime = millis();

	int newX = analogRead(JOYSTICK_X);
	
	if (thisTime - lastUpdate_X > DEBOUNCE_DELAY) {
		lastUpdate_X = thisTime;
		x = newX;
	}
	return newX;
}

bool readJoystick_Z(void) {

	unsigned long thisTime = millis();

	bool newZ = digitalRead(JOYSTICK_Z);
	
	if (thisTime - lastUpdate_Z > DEBOUNCE_DELAY) {
		lastUpdate_Z = thisTime;
		z = newZ;
	}
	return newZ;
}
