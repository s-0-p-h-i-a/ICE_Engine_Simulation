#include <stdbool.h>

#ifndef DRIVE_H
#define DRIVE_H

extern int currentLoop;

void checkEngineON(void);
void getSpeedInput(void);
bool getEngineStateCheckNow(void);
void checkEngineState(void);
int getFlywheelSpeed(void);
void driveEngine(void);

#endif
