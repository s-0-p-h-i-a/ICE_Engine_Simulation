#include "servolib.h"
#include <Arduino.h>
#include <Servo.h>

#define ZERO 0
#define SERVO_DELAY 15

static long unsigned int lastStep = ZERO;

Servo serv0;

void initServoSystem(void) {
	
	serv0.write(ZERO);
}

uint8_t servoCanStepNow(void) {
	
	unsigned long now = millis();
	
	uint8_t stepNow = now - lastStep > SERVO_DELAY;
	
	if (stepNow) { lastStep = now; };
	
	return stepNow;
}


void moveServo(int angle) {

	if (servoCanStepNow()) {
		serv0.write(angle);
	}
}


/**void initServoSystem(void) {
	
	serv0.write(ZERO);
}

void moveServo(void) {

	unsigned long now = millis();
	
	if (now - lastStep > SERVO_DELAY) {
		
		lastStep = now;
		spinFlywheel();
		serv0.write(flywheelAngle);
	}
}**/
