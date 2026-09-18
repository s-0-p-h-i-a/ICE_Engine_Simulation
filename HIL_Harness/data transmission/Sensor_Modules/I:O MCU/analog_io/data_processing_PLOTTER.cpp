#include "control_RED.h"
#include "control_GREEN.h"
#include "control_BLUE.h"
#include "data_interface.h"
#include "data_processing_PLOTTER.h"
#include <stdint.h>

#define POTENTIOMETER_INPUT_PROCESSING_PLOTTER 1
#define JOYSTICK_INPUT_PROCESSING_PLOTTER 5
#define WATER_LEVEL_SENSOR_INPUT_PROCESSING_PLOTTER 5

#define RED_BRIGHTNESS_LEVEL_PROCESSING_PLOTTER 5
#define RED_BLINK_PAUSE_PROCESSING_PLOTTER 5
#define GREEN_BRIGHTNESS_LEVEL_PROCESSING_PLOTTER 5
#define BLUE_BRIGHTNESS_LEVEL_PROCESSING_PLOTTER 5

uint16_t potentiometer_Input_PLOTTER = 0;
uint16_t joystick_Input_PLOTTER = 0;
uint16_t waterLevelSensor_Input_PLOTTER = 0;

uint8_t redBrightnessLevel_PLOTTER = 0;
uint8_t redPauseTime_PLOTTER = 0;
uint8_t greenBrightnessLevel_PLOTTER = 0;
uint8_t blueBrightnessLevel_PLOTTER = 0;

void computePlotterValues(void) {

	potentiometer_Input_PLOTTER 	= potentiometer_Input * POTENTIOMETER_INPUT_PROCESSING_PLOTTER;
	joystick_Input_PLOTTER 			= joystick_Input * JOYSTICK_INPUT_PROCESSING_PLOTTER;
	waterLevelSensor_Input_PLOTTER 	= waterLevelSensor_Input * WATER_LEVEL_SENSOR_INPUT_PROCESSING_PLOTTER;

	redBrightnessLevel_PLOTTER 		= redBrightnessLevel_Data * RED_BRIGHTNESS_LEVEL_PROCESSING_PLOTTER;
	redPauseTime_PLOTTER			= redPauseTime_Data * RED_BLINK_PAUSE_PROCESSING_PLOTTER;
	greenBrightnessLevel_PLOTTER	= greenBrightnessLevel_Data * GREEN_BRIGHTNESS_LEVEL_PROCESSING_PLOTTER;
	blueBrightnessLevel_PLOTTER 	= blueBrightnessLevel_Data * BLUE_BRIGHTNESS_LEVEL_PROCESSING_PLOTTER;
}
