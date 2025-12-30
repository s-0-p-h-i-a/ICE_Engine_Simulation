#ifndef FLYWHEEL_LIB_H
#define FLYWHEEL_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
	ANGLE_ZERO = 0,
	ANGLE_180 = 180
};

extern unsigned int flywheelAngle;

void initFlywheel(void);

void handleEdgeValues(void);

void spinFlywheel(int currentSpeed);

#ifdef __cplusplus
}
#endif

#endif
