#include "error_handling_JOYSTICK.h"
#include <stdint.h>
#include <arduino.h>

static const uint16_t ANALOG_MAX = 1023;
static const uint8_t MAX_ERRORS = 100;
static const uint16_t EXCESS_ERRORS_TIME_BUFFER = 1000;

static uint8_t joystickErrorCounter = 0;

static unsigned long lastErrorsExceededTime = 0;

static uint8_t joystickSystemStateArray[JOYSTICK_ERROR_TRACKING_ARRAY_SIZE] = { NO_ERRORS_CODE, NO_ERRORS_CODE };

static bool checkIfExcessErrorsBufferElapsed(void);
static void handleJoystickInputError(void);

static bool checkIfExcessErrorsBufferActive(void) {
	unsigned long timeNow = millis();
	bool bufferElapsed = timeNow - lastErrorsExceededTime > EXCESS_ERRORS_TIME_BUFFER;
	if (bufferElapsed) {
		joystickSystemStateArray[JOYSTICK_ERROR_TYPE] = NO_ERRORS_CODE;
		lastErrorsExceededTime = 0;
	}
	return bufferElapsed;
}

static void handleJoystickInputError(void) {
	
	++joystickSystemStateArray[JOYSTICK_ERROR_COUNTER];
	
	if (joystickErrorCounter == MAX_ERRORS) {
		joystickSystemStateArray[JOYSTICK_ERROR_COUNTER] = 0;
		lastErrorsExceededTime = millis();
		joystickSystemStateArray[JOYSTICK_ERROR_TYPE] = EXCESS_ERRORS_TIME_OUT_CODE;
		return;
	}
	
	joystickSystemStateArray[JOYSTICK_ERROR_TYPE] = INVALID_INPUT_CODE;
}

uint8_t getJoystickSystemStateArray(JoystickSystemStates index) {
	return joystickSystemStateArray[index];
}
	
bool checkJoystickSystemStatusValid(uint16_t joystickInput) {
	
	if (!checkIfExcessErrorsBufferActive()) {
		return 0;
	}
	
	if (joystickInput > ANALOG_MAX) {
		handleJoystickInputError();
		return 0;
	}

	return 1;
}
