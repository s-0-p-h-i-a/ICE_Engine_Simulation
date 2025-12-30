/*
 
 arduino-cli compile --fqbn arduino:avr:uno 1cylinder_v0.ino

06/12 FLYWHEEL + CYLINDER INTEGRATION: FIRST DRAFT

- Joystick now controls central engine = cylinder
- spinFlywheel now part of driveCar

 */



#include "global.h"
#include "joystick.h"
#include "hall_lib.h"
#include "servolib.h"

#include "flywheel_lib.h"
#include "engine.h"
#include "drive.h"

#include "rpmlib.h"
#include "engineVisuals.h"
#include "plotter_lib.h"

#include <Arduino.h>
#include <Servo.h>

void driveCar(void);

void setup() {

	startEngine();
	
	initFlywheel();
	
	initHall();

	serv0.attach(SERVO_PIN);
	
	pinMode(JOYSTICK_X, INPUT);
	pinMode(JOYSTICK_Y, INPUT);
	pinMode(JOYSTICK_Z, INPUT);
	
	pinMode(intakeValvePin, OUTPUT);
	pinMode(exhaustValvePin, OUTPUT);
	pinMode(fuelInjectorPin, OUTPUT);
	pinMode(sparkPlugPin, OUTPUT);
	pinMode(tdcPin1, OUTPUT);
	pinMode(tdcPin2, OUTPUT);
	pinMode(bdcPin1, OUTPUT);
	pinMode(bdcPin2, OUTPUT);
	
	Serial.begin(9600);
	
	initServoSystem();
}

void loop() {
	
	handleEdgeValues();
	
	readJoystick();
	
	checkEngineState();
	
	driveCar();
	
	displayEngine();
	
	getTimeWindow();
	
	getRPM();

	plotterDisplay();
	
	now = millis();		
}


void driveCar(void) {
	
	if (engineON) {
		
		driveEngine();
		
		spinFlywheel(speed);
		
		moveServo();
	}
}
