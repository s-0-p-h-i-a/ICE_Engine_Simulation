#include "joystick.h"
#include <stdint.h>
#include <arduino.h>

#define ZERO 0

//static const int ANALOG_CORRECTION = 128;

typedef enum { DEBOUNCE_DELAY = 100, BUTTON_DELAY = 1000 } Delays;

static int JOYSTICK_X = A5;

static unsigned int oldX = ZERO;
static uint8_t oldZ = 0;

static unsigned long lastUpdate_X = ZERO;
static unsigned long lastUpdate_Z = ZERO;

int readJoystick_X(void) {

	unsigned long thisTime = millis();

	int newX = oldX;
	
	if (thisTime - lastUpdate_X > DEBOUNCE_DELAY) {
		lastUpdate_X = thisTime;
		newX = analogRead(JOYSTICK_X);
		oldX = newX;
	}
	return newX;
}

uint8_t readJoystick_Z(void) {

	unsigned long thisTime = millis();

	uint8_t newZ = oldZ;
	
	if (thisTime - lastUpdate_Z > DEBOUNCE_DELAY) {
		lastUpdate_Z = thisTime;
		newZ = digitalRead(JOYSTICK_Z);
		oldZ = newZ;
	}
	return newZ;
}
