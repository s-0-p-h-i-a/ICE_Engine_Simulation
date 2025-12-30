#include <Arduino.h>
#include <Servo.h>

#ifndef SERVOLIB_H
#define SERVOLIB_H

extern Servo serv0;

void initServoSystem(void);

void advanceFlywheelAngle(int flywheelAngleStep);

#endif
