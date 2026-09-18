#include <stdint.h>

#ifndef DATA_INTERFACE_H
#define DATA_INTERFACE_H

extern uint16_t potentiometer_Input;
extern uint16_t joystick_Input;
extern uint16_t waterLevelSensor_Input;

extern uint8_t redBrightnessLevel_Data;
extern uint8_t redPauseTime_Data;
extern uint8_t greenBrightnessLevel_Data;
extern uint8_t blueBrightnessLevel_Data;

void getData(void);

#endif
