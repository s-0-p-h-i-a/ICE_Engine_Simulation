#include "potentiometer.h"
#include "control_BLUE.h"
#include "range_handling.h"
#include "arduino.h"
#include <stdint.h>

static uint16_t potentiometerInput = 0;
static uint8_t blueBrightness = 0;

static RangeErrorStatus blueRangeHandlingStatus = NO_RANGEHANDLING_ERRORS;

static void turnBlueLightOff(uint8_t bluePin);
static void controlBlueLightBrightness(uint8_t bluePin, uint8_t brightness);

static void turnBlueLightOff(uint8_t bluePin) {	
	digitalWrite(bluePin, LOW);	
}

static void controlBlueLightBrightness(uint8_t bluePin, uint8_t brightness) {
	digitalWrite(bluePin, brightness);
}

uint16_t getPotentiometerInput(void) {
	return potentiometerInput;
}

uint8_t getBlueBrightness(void) {
	return blueBrightness;
}

RangeErrorStatus getBlueRangeHandlingStatus(void) {
	return blueRangeHandlingStatus;
}

void controlBlueLight(uint8_t bluePin) {
	
	potentiometerInput = readPotentiometerInput(bluePin);
	
	blueBrightness = RGB_RANGE_MAX - clampInputToRGBRange(potentiometerInput, POTENTIOMETER_RANGE_STATUS);
	blueRangeHandlingStatus = getRangeHandlingStatus(POTENTIOMETER_RANGE_STATUS);
	
	if (blueBrightness < HYST) {
		turnBlueLightOff(bluePin);
	}
	else {
		controlBlueLightBrightness(bluePin, blueBrightness);
	}
}
