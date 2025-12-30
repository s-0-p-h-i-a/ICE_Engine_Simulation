#ifndef DRIVE_H
#define DRIVE_H

extern bool isIdle;
extern bool engineON;
extern bool speedOneInput;
extern bool speedTwoInput;

void startEngine(void);
void checkEngineON(void);
void getSpeedInput(void);
//int getServoSpeed(void);
int getCylinderPause(void);
void driveEngine(void);

#endif
