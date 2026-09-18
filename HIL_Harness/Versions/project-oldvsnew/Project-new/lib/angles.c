/**
Angles library:

- Typedef and constants
- Manage angle incrementation for crank module

**/

#include "./include/angles.h"
#include <stdint.h>

// DEFINITIONS
typedef uint8_t angle;

// CONSTANTS
static const angle MAX_ANGLE 	= 360;
static const angle ANGLE_TDC	= 0;
static const angle ANGLE_90		= 90;
static const angle ANGLE_BDC	= 180;
static const angle ANGLE_270	= 270;

// FUNCTIONS
// STATIC
// CLAMP+WRAP ANGLE
static angle clampAndWrapAngle(angle currentAngle) {
	
	angle resultAngle = currentAngle;
	
	if (resultAngle == MAX_ANGLE) {
		resultAngle = ANGLE_ZERO;
	}
	
	if (resultAngle > MAX_ANGLE) {
		resultAngle = resultAngle - MAX_ANGLE;
	}
	return resultAngle;
}

// NON STATIC
// UPDATE ANGLE
angle updateAngle(angle currentAngle, uint8_t increment) {
	
	angle newAngle = currentAngle + increment;
	
	newAngle = clampAndWrapAngle(newAngle);
	
	return newAngle;
}

// CHECK IF ANGLE VALID
bool checkAngleValid(angle angleToCheck) {
	
	bool invalidAngle = (angleToCheck < ANGLE_ZERO) || (angleToCheck > MAX_ANGLE);

	return invalidAngle;
}
