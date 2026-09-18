#include <stddef.h>

#ifndef ERROR_CODES_INTERFACE_H
#define ERROR_CODES_INTERFACE_H

typedef enum {	JOYSTICK_STATE = 0,
				JOYSTICK_ERROR_COUNTER,
				SYSTEM_STATE,
				RED,
				GREEN,
				BLUE } ReportArrayElements;

typedef enum { 	NO_FAULTS_CODE = 0,
				EXCESS_ERRORS_PAUSE_CODE = 1,
				INVALID_INPUT_CODE = 2 } JoystickSystemStates;
				
typedef enum {	ALL_MODULES_OK = 0,
				ONE_MODULE_HAS_ERROR = 1,
				TWO_MODULES_HAVE_ERROR = 2,
				THREE_MODULES_HAVE_ERROR = 3 } SystemErrors;

typedef enum { 	NO_RANGE_HANDLING_ERRORS 		= 0,	
				ANALOG_MAX_EXCEEDED 			= 1,
				RGB_MAX_EXCEEDED 				= 2,
				ANALOG_AND_RGB_MAX_EXCEEDED		= 3,
				UNDEFINED_RANGE_HANDLING_ERROR 	= 4 } RangeErrorStatus;
				
static const size_t STATES_ARRAY_SIZE = 3;
static const size_t SYSTEM_ERRORS_ARRAY_SIZE = 4;
static const size_t RANGE_ERRRORS_ARRAY_SIZE = 5;
														
const char *joystickSystemStateMessages[STATES_ARRAY_SIZE] =
		{ "NO_FAULTS_CODE", "EXCESS_ERRORS_PAUSE_CODE", "INVALID_INPUT_CODE" };

const char *systemErrorMessages[SYSTEM_ERRORS_ARRAY_SIZE] =
		{ "ALL_MODULES_OK", "1_MODULE_HAS_ERROR", "2_MODULES_HAVE_ERROR", "3_MODULES_HAVE_ERROR" };	
														
const char *rangeErrorStatusMessages[RANGE_ERRRORS_ARRAY_SIZE] =
		{ "NO_RANGE_HANDLING_ERRORS", "ANALOG_MAX_EXCEEDED", "RGB_MAX_EXCEEDED", "ANALOG_AND_RGB_MAX_EXCEEDED", "UNDEFINED_RANGE_HANDLING_ERROR" };
																							
#endif
				
