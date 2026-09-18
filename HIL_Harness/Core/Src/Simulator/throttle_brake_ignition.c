/**
Throttle, brake and ignition input:

Reads:
	- Throttle (potentiometer)
	- Brake (joystick)
	- Ignition on/off (joystick button press)

Processes and clamps throttle + brake data to percentage range, to be used to compute engine load.

**/

#include "throttle_brake_ignition.h"
#include "hw_data_interface.h"
#include <stdint.h>
#include <stdbool.h>

// DEFIITIONS
typedef int8_t hardware_input;
typedef uint8_t data_processing_values;

// CONSTANTS
static const hardware_input MAX_INPUT = 10;
static const data_processing_values JOYSTICK_SHIFT = 500;
static const data_processing_values ANALOG_TO_INPUT_RANGE_RATIO = 101;

// FUNCTIONS
// STATIC
// CLAMP INPUT LEVEL
static hardware_input clampInput(hardware_input input) {
	
	hardware_input clampedInput = input;
	
	if (input > MAX_INPUT) {
		clampedInput = MAX_INPUT;
	}
	return clampedInput;
}

// NON STATIC
// THROTTLE
hardware_input readThrottle(void) {
	
	uint16_t potInput = getPotentiometerInput() / ANALOG_TO_INPUT_RANGE_RATIO;
	hardware_input throttleInput = clampInput(potInput);
	
	return throttleInput;
}

// BRAKE
hardware_input readBrake(void) {
	
	uint16_t joystickInput = (getJoystickInput() - JOYSTICK_SHIFT) / ANALOG_TO_INPUT_RANGE_RATIO;
	hardware_input brakeInput = -clampInput(joystickInput);
	
	return brakeInput;
}

// CHECK ON/OFF TOGGLE
bool checkEngineToggle(void) {
	
	bool newToggle = getButtonInput();
	return newToggle;
}
