#include <Arduino.h>
#include <Servo.h>

#ifndef SERVOLIB_H
#define SERVOLIB_H

#define SERVO_PIN 4

extern Servo serv0;

void initServoSystem(void);

void moveServo(int angle); 

#endif
