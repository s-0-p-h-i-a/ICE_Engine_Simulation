#include "joystick.h"
#include "arduino.h"
#include <stdint.h>

uint16_t readJoystickInput(uint8_t joystickPin) {
	return analogRead(joystickPin);
}
