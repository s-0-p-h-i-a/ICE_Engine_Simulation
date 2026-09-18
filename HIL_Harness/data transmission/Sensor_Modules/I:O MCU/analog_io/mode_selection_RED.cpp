/**

Joystick input modes:
- Neutral (±511) = neutral mode: red OFF
- Up (±511 - 1023) -> mode 1: red goes from 0 to 255
- Down (±511 - 0) -> mode 2: red blinks progressively faster, delay 1000ms to 20 ms

**/

#include "mode_selection_RED.h"
#include "range_handling.h"
#include <stdint.h>

static const uint16_t ANALOG_RANGE_MAX = 1023;
static const uint16_t JOYSTICK_NEUTRAL = 512;
static const uint16_t NEUTRAL_RANGE_START = JOYSTICK_NEUTRAL - HYST;
static const uint16_t NEUTRAL_RANGE_END = JOYSTICK_NEUTRAL + HYST;
static bool isJoystickStateValid = 1;

static bool checkIfJoystickStateValid(bool neutralMode, bool modeOne, bool modeTwo);

static bool checkIfJoystickStateValid(bool neutralMode, bool modeOne, bool modeTwo) {
	return neutralMode || modeOne || modeTwo;
	}

JoystickModes computeJoystickMode(uint16_t joystickInput) {
	
	if (joystickInput > ANALOG_RANGE_MAX) {
		return ERROR;
	}
	
	bool neutralModeON 	= (joystickInput >= NEUTRAL_RANGE_START) && (joystickInput <= NEUTRAL_RANGE_END);
	bool mode1ON 		= joystickInput > NEUTRAL_RANGE_END;
	bool mode2ON 		= joystickInput < NEUTRAL_RANGE_START;
	
	isJoystickStateValid = checkIfJoystickStateValid(neutralModeON, mode1ON, mode2ON);
	
	if (!isJoystickStateValid) {
		return ERROR;
	}
	
	if (neutralModeON) {
		return NEUTRAL;
	}
	else if (mode1ON) {
		return MODE1;
	}
	else if (mode2ON) {
		return MODE2;
	}
}
