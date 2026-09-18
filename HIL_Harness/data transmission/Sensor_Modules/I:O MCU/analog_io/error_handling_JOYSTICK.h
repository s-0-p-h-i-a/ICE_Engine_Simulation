#include <stddef.h>
#include <stdint.h>

#ifndef ERROR_HANDLING_JOYSTICK_H
#define ERROR_HANDLING_JOYSTICK_H
				
typedef enum {	NO_ERRORS_CODE 				= 0,
				INVALID_INPUT_CODE 			= 1,
				EXCESS_ERRORS_TIME_OUT_CODE = 2 } JoystickSystemStates;

typedef enum {	JOYSTICK_ERROR_TYPE   = 0,
				JOYSTICK_ERROR_COUNTER = 1 } JoystickErrorMatrixPositions;
				
const size_t JOYSTICK_ERROR_TRACKING_ARRAY_SIZE = 2;
	
extern uint8_t getJoystickSystemStateArray(JoystickSystemStates index);			
extern bool checkJoystickSystemStatusValid(uint16_t joystickInput);

#endif
