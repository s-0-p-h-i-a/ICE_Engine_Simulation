#include <stdint.h>

#ifndef MODE_SELECTION_RED_H
#define MODE_SELECTION_RED_H

typedef enum { NEUTRAL = 0, MODE1 = 1, MODE2 = 2, ERROR = 3 } JoystickModes;

extern JoystickModes computeJoystickMode(uint16_t joystickInput);

#endif
