#include <stdint.h>

#ifndef JOYSTICK_H
#define JOYSTICK_H

enum {
	JOYSTICK_Z	= 3,
};

int readJoystick_X(void);
uint8_t readJoystick_Z(void);

#endif
