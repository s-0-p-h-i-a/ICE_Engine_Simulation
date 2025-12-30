#include "flywheel_libTEST.h"
#include <stdbool.h>

#define ZERO 0

enum {
	ANGLE_ZERO = 0,
	ANGLE_180 = 180
};

static bool sweepBack = 0;

int flywheelAngle = ZERO;
int flywheelSpeed = ZERO;

void handleEdgeValues(void) {
	
	// prevent angle from going beyond 0-180
	if (flywheelAngle < ANGLE_ZERO) { 
		flywheelAngle = ANGLE_ZERO;
		sweepBack = 1; }
			
	if (flywheelAngle > ANGLE_180) {
		flywheelAngle = ANGLE_180;
		sweepBack = 1; }	
}

bool checkDirectionToggle(void) {
	
	bool toggleSpeedZero = (flywheelAngle == ANGLE_ZERO) && (flywheelSpeed < 0);
	bool toggleSpeed180  = (flywheelAngle == ANGLE_180)  && (flywheelSpeed > 0);
			
	bool toggleNow = toggleSpeedZero || toggleSpeed180;
	sweepBack = toggleNow;
	
	return toggleNow;
}

void spinFlywheel(int speed) {
	
	handleEdgeValues();
	
	checkDirectionToggle();
	
	if (sweepBack) { flywheelSpeed = -flywheelSpeed; }
	
	flywheelSpeed = (flywheelSpeed >= ZERO) ? speed : -speed;
	
	flywheelAngle += flywheelSpeed;
}
