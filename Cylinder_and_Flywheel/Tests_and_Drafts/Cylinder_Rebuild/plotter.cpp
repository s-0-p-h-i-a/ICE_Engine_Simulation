#include "drive.h"
#include "joystick.h"
#include "plotter.h"
#include <arduino.h>

void plotterDisplay(void) {
	Serial.print(engineON*300 + 50);
	Serial.print(" ");
	Serial.print(isIdle*200);
	Serial.print(" ");
	Serial.print(readJoystick_Z()*100);
	Serial.print(" ");
	Serial.print(readJoystick_X());
	Serial.print(" ");
	Serial.print(speedOneInput*150);
	Serial.print(" ");
	Serial.println(speedTwoInput*150);
}
