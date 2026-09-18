#include "control_RED.h"
#include "control_GREEN.h"
#include "control_BLUE.h"
#include "data_interface.h"
#include <stdint.h>

uint16_t potentiometer_Input = 0;
uint16_t joystick_Input = 0;
uint16_t waterLevelSensor_Input = 0;

uint8_t redBrightnessLevel_Data = 0;
uint8_t redPauseTime_Data = 0;
uint8_t greenBrightnessLevel_Data = 0;
uint8_t blueBrightnessLevel_Data = 0;

void getData(void) {

	potentiometer_Input 	= getPotentiometerInput();
	joystick_Input 			= getJoystickInput();
	waterLevelSensor_Input	= getWaterLevelSensorInput();

	redBrightnessLevel_Data 	= getRedBrightness();
	redPauseTime_Data			= getRedBlinkPause();
	greenBrightnessLevel_Data	= getGreenBrightness();
	blueBrightnessLevel_Data 	= getBlueBrightness();
}
