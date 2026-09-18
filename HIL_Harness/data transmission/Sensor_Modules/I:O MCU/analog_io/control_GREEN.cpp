#include "water_level_sensor.h"
#include "control_GREEN.h"
#include "range_handling.h"
#include "arduino.h"
#include <stdint.h>

static uint16_t waterLevelSensorInput = 0;
static uint8_t greenBrightness = 0;

static RangeErrorStatus greenRangeHandlingStatus = NO_RANGEHANDLING_ERRORS;

static void turnGreenLightOff(uint8_t greenPin);
static void controlGreenLightBrightness(uint8_t bluePin, uint8_t brightness);

static void turnGreenLightOff(uint8_t greenPin) {
	digitalWrite(greenPin, LOW);	
}

static void controlGreenLightBrightness(uint8_t greenPin, uint8_t brightness) {
	digitalWrite(greenPin, brightness);
}

uint16_t getWaterLevelSensorInput(void) {
	return waterLevelSensorInput;
}

uint8_t getGreenBrightness(void) {
	return greenBrightness;
}

RangeErrorStatus getGreenRangeHandlingStatus(void) {
	return greenRangeHandlingStatus;
}

void controlGreenLight(uint8_t greenPin) {
	
	waterLevelSensorInput = readWaterLevelSensorInput(greenPin);
	greenBrightness = clampInputToRGBRange(waterLevelSensorInput, WATER_LEVEL_SENSOR_RANGE_STATUS);
	greenRangeHandlingStatus = getRangeHandlingStatus(WATER_LEVEL_SENSOR_RANGE_STATUS);
	
	if (greenBrightness < HYST) {
		turnGreenLightOff(greenPin);
	}
	else {
		controlGreenLightBrightness(greenPin, greenBrightness);
	}
}


