#include <stdint.h>

#ifndef RANGE_HANDLING_H
#define RANGE_HANDLING_H

typedef enum { 	NO_RANGEHANDLING_ERRORS 		= 0,	
				ANALOGMAX_EXCEEDED 				= 1,
				RGBMAX_EXCEEDED 				= 2,
				ANALOG_AND_RGBMAX_EXCEEDED		= 3,
				UNDEFINED_RANGEHANDLING_ERROR 	= 4 } RangeErrorStatus;

typedef enum { 	JOYSTICK_RANGE_STATUS = 0,
				WATER_LEVEL_SENSOR_RANGE_STATUS,
				POTENTIOMETER_RANGE_STATUS } RangeHandlingArrayElements;
				
const uint8_t HYST = 50;
const uint8_t RGB_RANGE_MAX = 255;

extern RangeErrorStatus getRangeHandlingStatus(uint8_t inputType);
extern uint8_t clampInputToRGBRange(uint16_t sensorInput, uint8_t inputType);

#endif
