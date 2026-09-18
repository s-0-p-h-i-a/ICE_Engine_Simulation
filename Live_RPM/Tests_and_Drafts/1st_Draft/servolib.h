#include <Arduino.h>
#include <Servo.h>

#ifndef SERVOLIB_H
#define SERVOLIB_H

enum {
	SERVO_PIN 	= 11,
	SERVO_DELAY = 15
};

extern Servo serv0;

void initServoSystem(void);

void moveServo(void); 

#endif
