/*
RPM Calculation System:
Servo Control Module

Function / Role:
- Moves servo based on flywheel angle input

Process:
- Checks if servo response period is elapsed
- If servo ready to move again:
	- Reads and interprets flywheel angle input:
		- 0° - 180° angle input interpreted as is
		- > 180° flywheel angle input mirrored to produce backward sweep
	- Writes servo to interpreted angle
	
System layers (top -> bottom):
flywheel_ref_lib.cpp
servo_lib.cpp
hall_lib.cpp
rpm_lib.cpp

*/

#include "servolib.h"
#include <Arduino.h>
#include <Servo.h>

//	========= CONSTANTS =========
enum {
	ZERO		= 0,
	SERVO_PIN	= 4,
	SERVO_DELAY	= 15,
	ANGLE_180 	= 180
};

// 	========= VARIABLES =========
// Stores last servo move timestamp
static long unsigned int lastServoStep = ZERO;
// Servo
Servo serv0;

//	========= FUNCTIONS =========
// Helper functions
static bool servoCanStepNow(void);
static unsigned int mapFlywheelToServoAngle(unsigned int flywheelAngle);
static unsigned int safeServoAngle(unsigned int servoAngle);

// Attaches servo + sets to 0° when the system is initialised
void initServoSystem(void) {
	
	serv0.attach(SERVO_PIN);
	serv0.write(ANGLE_ZERO);
}

// Checks if servo response period is elapsed
static bool servoCanStepNow(void) {
	
	unsigned long now = millis();
	
	bool canStepNow = (now - lastServoStep) > SERVO_DELAY;
	
	// Updates timestamp for next servo step
	if (canStepNow) { lastServoStep = now; }
	
	return canStepNow;
}

// Wraps flywheel angle into [0°, 360°), mirrors if > 180°
static unsigned int mapFlywheelToServoAngle(unsigned int flywheelAngle) {
    
    unsigned int servoAngle = flywheelAngle % ANGLE_360;

    if (servoAngle <= ANGLE_180) {
        return servoAngle;
    } else {
        return ANGLE_360 - servoAngle;
    }
}

// Last safety layer
static unsigned int safeServoAngle(unsigned int servoAngle) {
    if (servoAngle > ANGLE_180) {
        return ANGLE_180;
    }
    return servoAngle;
}

// Checks if servo is ready for next step, writes interpreted angle input
void moveServo(unsigned int flywheelAngleInput) {

	if (servoCanStepNow()) {
		unsigned int servoAngle = mapFlywheelToServoAngle(flywheelAngleInput);

        servoAngle = safeServoAngle(servoAngle);

        serv0.write(servoAngle);
	}
}
