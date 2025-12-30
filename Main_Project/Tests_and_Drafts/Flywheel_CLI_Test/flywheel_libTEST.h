#include <stdbool.h>

#ifndef FLYWHEEL_LIBTEST_H
#define FLYWHEEL_LIBTEST_H

extern int flywheelAngle;
extern int flywheelSpeed;

void initFlywheel(void);

void handleEdgeValues(void);

bool checkDirectionToggle(void);

void spinFlywheel(int speed);

#endif
