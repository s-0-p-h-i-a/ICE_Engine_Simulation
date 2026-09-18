// ENUMS / DEFINES / CONSTANTS
typedef uint8_t angle_position;

static const angle_position MAX_ANGLE 	= 360;
static const angle_position ANGLE_ZERO	= 0;


static angle_position wrapAngle(angle_position currentAngle) {
	
	angle_position wrappedAngle = currentAngle;
	
	if (wrappedAngle == MAX_ANGLE) {
		wrappedAngle = ANGLE_ZERO;
	}
	
	if (wrappedAngle > MAX_ANGLE) {
		wrappedAngle = wrappedAngle - MAX_ANGLE;
	}
	return wrappedAngle;
}

angle_position updateAngle(angle_position currentAngle, uint8_t increment) {
	
	angle_position newAngle = currentAngle;
	
	newAngle += increment;
	
	newAngle = wrapAngle(newAngle);
	
	return newAngle;
}

bool checkAngleValid(angle_position angleToCheck) {
	
	bool invalidAngle = (angleToCheck < ANGLE_ZERO) || (angleToCheck > MAX_ANGLE);

	return invalidAngle;
}
