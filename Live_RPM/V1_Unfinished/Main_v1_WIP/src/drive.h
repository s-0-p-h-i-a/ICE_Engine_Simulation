#include <stdint.h>

#ifndef DRIVE_H
#define DRIVE_H

extern uint8_t isIdle;
extern uint8_t engineON;
extern uint8_t speedOneInput;
extern uint8_t speedTwoInput;

void startEngine(void);
void checkEngineON(void);
void getSpeedInput(void);
void idleCheck(void);
void checkEngineState(void);
int getFlywheelSpeed(void);
int getCylinderPause(void);
void driveEngine(void);

#endif
