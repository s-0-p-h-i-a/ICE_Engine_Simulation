/**
Hardware interface:

- Reads input from potentiometer, joystick and joystick button and clamps to analog max
- Lets relevant modules know if new input has been received and the value of the new input

**/

#include "hw_interface.h"
#include <stdint.h>
#include <stdbool.h>

// DEFINITIONS
typedef uint8_t pin_number;
typedef uint16_t input_level;
typedef unsigned long time;

// CONSTANTS
static const pin_number POT_PIN = 4;
static const pin_number JOYSTICK_PIN = 5;
static const pin_number BUTTON_PIN = 6; // random pin numbers for compile check
static const input_level ZERO_INPUT = 0;
static const input_level ANALOG_MAX = 1023;

static const time TIMER_ZERO = 0;
static const time DEBOUNCE_TIME = 100;

// VARIABLES
static input_level potentiometerLevel = ZERO_INPUT;
static input_level joystickLevel = ZERO_INPUT;
static bool buttonState = 0;
static time lastPotRead = TIMER_ZERO;
static time lastJoystickRead = TIMER_ZERO;
static time lastButtonRead = TIMER_ZERO;
static time timeNow = TIMER_ZERO;

// FUNCTIONS
// STATIC
static input_level clampToAnalogMax(void);
static bool debounceHardware(void);
static input_level readPotentiometer(void);
static input_level readJoystick(void);
static bool readButton(void)

// ANALOG CLAMP
static input_level clampToAnalogMax(input_level newInput) {
	
	if (newInput > ANALOG_MAX) {
		newInput = ANALOG_MAXL
	}
	return newInput;
}

// DEBOUNCE
static bool debounceTimeElapsed(time lastReadEvent) {
	
	timeNow = HAL_GetTick();
	bool timeElapsed = (timeNow - lastReadEvent) > DEBOUNCE_TIME;
	return timeElapsed;
}	

// READ POT
static input_level readPotentiometer(void) {
	
	if (debounceTimeElapsed(lastPotRead)) {
		
		lastPotRead = timeNow;
		potentiometerLevel = HAL_GPIO_ReadPin(GPIOA, POT_PIN);	
	}
	potentiometerLevel = clampToAnalogMax(potentiometerLevel);
	return potentiometerLevel;
}

// READ JOYSTICK
static input_level readJoystick(void) {
	
	if (debounceTimeElapsed(lastButtonRead)) {
		
		lastButtonRead = timeNow;
		joystickLevel = HAL_GPIO_ReadPin(GPIOA, JOYSTICK_PIN);
	}
	joystickLevel = clampToAnalogMax(joystickLevel);
	return joystickLevel;
}

// READ BUTTON
bool readButton(void) {
	if (debounceTimeElapsed(lastJoystickRead)) {
		
		lastJoystickRead = timeNow;
		buttonState = HAL_GPIO_ReadPin(GPIOA, BUTTON_PIN);
	}
	return buttonState;
}

// NON STATIC
// NEW INPUT CHECK
// button input handled separately as button-controlled state toggle overrides rest of program execution
bool newHardwareInputReceived(void) {
	
	input_level lastPotInput = potentiometerLevel;
	input_level lastJoystickInput = joystickLevel;
	
	readPotentiometer();
	readJoystick();
	
	bool newPotInput = lastPotInput != potentiometerLevel;
	bool newJoystickInput = lastJoystickInput != joystickLevel;
	
	bool newInputReceived = newPotInput || newJoystickInput;
	
	return newInputReceived;
}

// GETTERS
// POTENTIOMETER
input_level getPotentiometerInput(void) {
	return potentiometerLevel;
}

// JOYSTICK
input_level getJoystickInput(void) {
	return joystickLevel;
}

// BUTTON
bool getButtonInput(void) {
	return buttonStateThisLoop;
}
