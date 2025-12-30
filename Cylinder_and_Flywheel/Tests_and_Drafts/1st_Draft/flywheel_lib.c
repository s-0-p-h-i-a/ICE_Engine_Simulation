#include "global.h"
#include "flywheel_lib.h"
#include "engine.h"
#include <stdbool.h>

unsigned int flywheelAngle = ZERO;
static unsigned int lastAngle = ZERO;

void initFlywheel(void) {
	
	flywheelAngle = ANGLE_ZERO;
}

void handleEdgeValues(void) {

	if (flywheelAngle == lastAngle) {;}

	else {
		// prevent angle from going beyond 0-180
		if (flywheelAngle < ANGLE_ZERO) { 
			lastAngle = flywheelAngle;
			flywheelAngle = ANGLE_ZERO;}
			
		if (flywheelAngle > ANGLE_180) {
			lastAngle = flywheelAngle;
			flywheelAngle = ANGLE_180;}
		
		// sweep back		
		if (flywheelAngle == ANGLE_ZERO || flywheelAngle == ANGLE_180) {
			speed = -speed;
		}
	}
}


void spinFlywheel(int currentSpeed) {
	
		lastAngle = flywheelAngle;
	 	flywheelAngle += currentSpeed;
}
