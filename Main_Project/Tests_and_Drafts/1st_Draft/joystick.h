#ifndef JOYSTICK_H
#define JOYSTICK_H

enum {
  JOYSTICK_X	= 0,
  JOYSTICK_Y	= 1,
  JOYSTICK_Z	= 7,
  DEBOUNCE_DELAY	= 100,
  ANALOG_CORRECTION = 128,
  BUTTON_DELAY 		= 1000
};

extern int x;
extern bool z;
extern unsigned long lastUpdate_X;
extern unsigned long lastUpdate_Z;

void readJoystick(void);

#endif
