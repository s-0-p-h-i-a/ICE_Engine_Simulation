#include "range_handling.h"
#include <stdint.h>

#ifndef CONTROL_BLUE_H
#define CONTROL_BLUE_H

extern uint16_t getPotentiometerInput(void);

extern uint8_t getBlueBrightness(void);

extern RangeErrorStatus getBlueRangeHandlingStatus(void);

extern void controlBlueLight(uint8_t bluePin);

#endif
