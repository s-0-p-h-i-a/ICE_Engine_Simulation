#include "global.h"
#include "flywheel_lib.h"
#include "servolib.h"
#include <Arduino.h>
#include <Servo.h>

static long unsigned int lastStep = ZERO;

Servo serv0;

void initServoSystem(void) {
	
	serv0.write(ANGLE_ZERO);
}

void moveServo(void) {
	
	if (now - lastStep >= SERVO_DELAY) {
		
		// update timestamp
		lastStep = now;
			
		serv0.write(flywheelAngle);
	}
}
