#include "range_handling.h"
#include <stdint.h>

#ifndef CONTROL_GREEN_H
#define CONTROL_GREEN_H

uint16_t getWaterLevelSensorInput(void);

uint8_t getGreenBrightness(void);

RangeErrorStatus getGreenRangeHandlingStatus(void);

extern void controlGreenLight(uint8_t greenPin);

#endif
