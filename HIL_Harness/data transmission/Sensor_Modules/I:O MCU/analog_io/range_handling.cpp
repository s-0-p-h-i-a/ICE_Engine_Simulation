#include "range_handling.h"
#include <stdint.h>
#include <stddef.h>
#include <math.h>			

static const uint16_t ANALOG_RANGE_MAX = 1023;
static const double ANALOG_TO_RGB_RATIO = RGB_RANGE_MAX / ANALOG_RANGE_MAX;
static const uint8_t RANGE_HANDLING_STATUSES_ARRAY_SIZE = 3;

static bool rangeExceededAnalog = 0;
static bool rangeExceededRGB = 0;
static RangeErrorStatus rangeHandlingStatuses[RANGE_HANDLING_STATUSES_ARRAY_SIZE] = { NO_RANGEHANDLING_ERRORS, 												NO_RANGEHANDLING_ERRORS, NO_RANGEHANDLING_ERRORS };

static uint16_t limitToAnalogMax(uint16_t analogInput);
static uint8_t limitToRGBMax(uint16_t inputValue);
static RangeErrorStatus checkRangeHandlingStatus(void);

static uint16_t limitToAnalogMax(uint16_t analogInput) {
	uint16_t result = analogInput;
	rangeExceededAnalog = analogInput > ANALOG_RANGE_MAX;
	if (rangeExceededAnalog) {
		result = ANALOG_RANGE_MAX;
	}
	return result;
}

static uint8_t limitToRGBMax(uint16_t inputValue) {	
	uint8_t result = RGB_RANGE_MAX;
	rangeExceededRGB = inputValue > RGB_RANGE_MAX;
	if (!rangeExceededRGB) {
		result = inputValue;
	}
	return result;
}

static RangeErrorStatus checkRangeHandlingStatus(void) {
	
	bool notCapped 			= !(rangeExceededAnalog || rangeExceededRGB);
	bool analogCappedOnly 	= rangeExceededAnalog && !rangeExceededRGB;
	bool rgbCappedOnly 		= !rangeExceededAnalog && rangeExceededRGB;
	bool analogAndRGBCapped = rangeExceededAnalog && rangeExceededRGB;
	
	if (notCapped) {
		return NO_RANGEHANDLING_ERRORS;
	} else if (analogCappedOnly) {
		return ANALOGMAX_EXCEEDED;
	} else if (rgbCappedOnly) {
		return RGBMAX_EXCEEDED;
	} else if (analogAndRGBCapped) {
		return ANALOG_AND_RGBMAX_EXCEEDED;
	} else {
		return UNDEFINED_RANGEHANDLING_ERROR;
	}
}

RangeErrorStatus getRangeHandlingStatus(uint8_t inputType) {
	return rangeHandlingStatuses[inputType];
}

uint8_t clampInputToRGBRange(uint16_t sensorInput, uint8_t inputType) {
	
	uint16_t input = limitToAnalogMax(sensorInput);
	uint8_t result = limitToRGBMax(round(input * ANALOG_TO_RGB_RATIO));
	
	rangeHandlingStatuses[inputType] = checkRangeHandlingStatus();
	
	return result;
}
