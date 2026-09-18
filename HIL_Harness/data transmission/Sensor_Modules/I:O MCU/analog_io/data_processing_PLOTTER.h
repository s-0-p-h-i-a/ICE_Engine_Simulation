#include <stdint.h>

#ifndef DATA_PROCESSING_PLOTTER_H
#define DATA_PROCESSING_PLOTTER_H

extern uint16_t potentiometer_Input_PLOTTER;
extern uint16_t joystick_Input_PLOTTER;
extern uint16_t waterLevelSensor_Input_PLOTTER;

extern uint8_t redBrightnessLevel_PLOTTER;
extern uint8_t redPauseTime_PLOTTER;
extern uint8_t greenBrightnessLevel_PLOTTER;
extern uint8_t blueBrightnessLevel_PLOTTER;

void computePlotterValues(void); 

#endif
