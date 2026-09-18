#include "global.h"
#include "joystick.h"
#include <arduino.h>

int x = ZERO;
bool z = false;
unsigned long lastUpdate_X = ZERO;
unsigned long lastUpdate_Z = ZERO;

void readJoystick(void) {

	int newX = analogRead(JOYSTICK_X);
	bool newZ = digitalRead(JOYSTICK_Z) > ZERO;
	
	if (now-lastUpdate_X > DEBOUNCE_DELAY) {
		
		lastUpdate_X = now;
		x = newX;
	}
	
	if (now-lastUpdate_Z > DEBOUNCE_DELAY) {
		
		lastUpdate_Z = now;
		z = newZ;
	}
}
