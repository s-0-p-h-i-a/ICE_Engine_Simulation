/**
Hardware interface:

- Reads input from potentiometer, joystick and joystick button and clamps to analog max
- Lets relevant modules know if new input has been received and the value of the new input

**/

#include "hw_interface.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

// DEFINITIONS
typedef uint8_t pin_number;
typedef uint32_t analog_channel;
typedef uint16_t input_level;
typedef unsigned long time;

typedef enum {	HARDWARE_INTERFACE_OK = 0,
				CHANNEL_CONFIG_ERROR } HardwareInterfaceStatuses;

// CONSTANTS
static const analog_channel POTENTIOMETER_CHANNEL = ADC_CHANNEL_1;
static const analog_channel JOYSTICK_CHANNEL = ADC_CHANNEL_2;
static const pin_number BUTTON_PIN = GPIO_PIN_12;
 
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
static HardwareInterfaceStatuses hardwareInterfaceStatusThisLoop = HARDWARE_INTERFACE_OK;
static analog_channel currentADCChannel = POTENTIOMETER_CHANNEL;

// FUNCTIONS
// STATIC
static input_level clampToAnalogMax(void);
static bool configureADCChannel(analog_channel channelToRead);
static input_level readADCChannel(void);
static bool debounceHardware(void);
static input_level readPotentiometer(void);
static input_level readJoystick(void);
static bool readButton(void);

// ANALOG CLAMP
static input_level clampToAnalogMax(input_level newInput) {
	
	if (newInput > ANALOG_MAX) {
		newInput = ANALOG_MAX;
	}
	return newInput;
}

// CHANNEL RECONFIG
static bool configureADCChannel(analog_channel channelToRead) {
	
	if ((channelToRead != ADC_CHANNEL_1) && (channelToRead != ADC_CHANNEL_2)) {
		hardwareInterfaceStatusThisLoop = CHANNEL_CONFIG_ERROR;
		return 0;
	}
	
	if (channelToRead != currentADCChannel) {
		currentADCChannel = channelToRead;
		ADC_ChannelConfTypeDef sConfig = {0};
		sConfig.Channel = currentADCChannel;
		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
		HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	}
	hardwareInterfaceStatusThisLoop = HARDWARE_INTERFACE_OK;
	return 1;
}

// READ CHANNEL
static input_level readADCChannel(void) {
	
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	input_level ADCValue = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	
	input_level readValue = clampToAnalogMax(ADCValue);
	return readValue;
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
		if (configureADCChannel(POTENTIOMETER_CHANNEL)) {
		
			lastPotRead = timeNow;
			potentiometerLevel = readADCChannel();
		}
	}
	return potentiometerLevel;
}

// READ JOYSTICK
static input_level readJoystick(void) {
	
	if (debounceTimeElapsed(lastJoystickRead)) {
		if (configureADCChannel(JOYSTICK_CHANNEL)) {
		
			lastJoystickRead = timeNow;
			joystickLevel = readADCChannel();
		}
	}
	return joystickLevel;
}

// READ BUTTON
bool readButton(void) {
	if (debounceTimeElapsed(lastJoystickRead)) {
		
		lastButtonRead = timeNow;
		buttonState = HAL_GPIO_ReadPin(GPIOB, BUTTON_PIN);
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
	return buttonState;
}
