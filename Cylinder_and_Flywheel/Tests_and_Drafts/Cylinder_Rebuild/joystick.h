#ifndef JOYSTICK_H
#define JOYSTICK_H

enum {
  JOYSTICK_X	= 0,
  JOYSTICK_Y	= 1,
  JOYSTICK_Z	= 7
};

int readJoystick_X(void);
bool readJoystick_Z(void);

#endif
