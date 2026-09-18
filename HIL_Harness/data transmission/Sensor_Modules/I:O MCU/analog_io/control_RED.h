#include "range_handling.h"
#include <stdint.h>

#ifndef CONTROL_RED_H
#define CONTROL_RED_H

extern uint16_t getJoystickInput(void);
extern uint8_t getRedBrightness(void);
extern uint16_t getRedBlinkPause(void);

extern RangeErrorStatus getRedRangeHandlingStatus(void);

extern void controlRedLight(uint8_t redPin);

#endif
