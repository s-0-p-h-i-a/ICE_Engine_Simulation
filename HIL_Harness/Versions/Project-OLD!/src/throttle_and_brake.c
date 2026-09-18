#include "throttle_and_brake.h"
#include "hw_data_interface.h"
#include <stdint.h>
#include <stdbool.h>

// CONSTANTS
static const uint8_t MAX_PERCENT = 100;
static const uint8_t JOYSTICK_SHIFT = 500;
static const uint8_t ANALOG_TO_PERCENT_RATIO = 10;

// FUNCTIONS
static uint8_t clampInput(uint8_t input) {
	
	uint8_t clampedInput = input;
	
	if (input > MAX_PERCENT) {
		clampedInput = MAX_PERCENT;
	}
	return clampedInput;
}

uint8_t readThrottle(void) {
	
	uint16_t potInput = getPotentiometerInput();
	
	uint8_t throttleInput = clampInput(potInput / ANALOG_TO_PERCENT);
	
	return throttleInput;
}

uint8_t readBrake(void) {
	
	uint16_t joystickInput = getJoystickInput() - JOYSTICK_SHIFT;
	
	uint8_t brakeInput = clampInput(joystickInput / ANALOG_TO_PERCENT);
	
	return brakeInput;
}
