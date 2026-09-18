#include "potentiometer.h"
#include "arduino.h"
#include <stdint.h>

uint16_t readPotentiometerInput(uint8_t potentiometerPin) {
	return analogRead(potentiometerPin);
}
