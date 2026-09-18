/**
NOTES:
- How to handle logic in controlRedLight?
- Option 1 switch branch, option 2 nested if/else
- Switch branch: flat structure, no logic layers, but all cases treated the same
- Solution: if branch for invalid input, else: switch branch with valid input modes

**/

#include "mode_selection_RED.h"
#include "joystick.h"
#include "control_RED.h"
#include "range_handling.h"
#include "error_handling_JOYSTICK.h"
#include "arduino.h"
#include <stdint.h>

static const uint8_t CONVERT_BRIGHTNESS_TO_PAUSE = 4;

static bool redLightON = 0;

static uint16_t joystick_X_Input = 0;
static uint16_t blinkPause = 0;
static uint8_t redBrightness = 0;

static RangeErrorStatus redRangeHandlingStatus = NO_RANGEHANDLING_ERRORS;

static unsigned long lastTime = 0;

static void turnRedLightOff(uint8_t redPin);
static void controlRedLightBrightness(uint8_t redPin, uint8_t brightness);
static void controlRedLightToggle(uint8_t redPin, uint8_t blinkPauseInput);
static void processModeSelection(uint8_t mode, uint16_t input);
	
static void turnRedLightOff(uint8_t redPin) {
	digitalWrite(redPin, LOW);	
}

static void controlRedLightBrightness(uint8_t redPin, uint8_t brightness) {
	digitalWrite(redPin, brightness);
}

static void controlRedLightToggle(uint8_t redPin, uint8_t blinkPauseInput) {

	blinkPause = blinkPauseInput * CONVERT_BRIGHTNESS_TO_PAUSE;
	unsigned long currentTime = millis();
	unsigned long delta = currentTime - lastTime;
	lastTime = currentTime;
	bool deltaElapsed = delta > blinkPause;
	
	if (deltaElapsed) {
		if (redLightON) {
			digitalWrite(redPin, LOW);
		} else {
			digitalWrite(redPin, HIGH);
		}
		redLightON = !redLightON;
	}
}

static void processModeSelection(uint8_t redPin, uint8_t mode, uint16_t input) {
	redBrightness = clampInputToRGBRange(input, JOYSTICK_RANGE_STATUS);
	redRangeHandlingStatus = getRangeHandlingStatus(JOYSTICK_RANGE_STATUS);
	blinkPause = 0;
	switch (mode) {
			case NEUTRAL: {
				turnRedLightOff(redPin);
			} break;
			case MODE1: {
				controlRedLightBrightness(redPin, redBrightness);
			} break;
			case MODE2: {
				controlRedLightToggle(redPin, redBrightness);
				redBrightness = RGB_RANGE_MAX;
			} break;
			case ERROR: {
				turnRedLightOff(redPin);
				return;
			}
	}
}

uint16_t getJoystickInput(void) {
	return joystick_X_Input;
}

uint8_t getRedBrightness(void) {
	return redBrightness;
}

uint16_t getRedBlinkPause(void) {
	return blinkPause;
}

RangeErrorStatus getRedRangeHandlingStatus(void) {
	return redRangeHandlingStatus;
}

void controlRedLight(uint8_t redPin) {
	
	joystick_X_Input = readJoystickInput(redPin);
	
	if (checkJoystickSystemStatusValid(joystick_X_Input)) {
		
		JoystickModes joystickMode = computeJoystickMode(joystick_X_Input);
		
		processModeSelection(redPin, joystickMode, joystick_X_Input);
	}
}
