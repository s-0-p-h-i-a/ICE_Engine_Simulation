#include "joystick.h"
#include "cylinder.h"
#include "cylinderVisuals.h"
#include "flywheel_lib.h"
#include "servolib.h"
#include "drive.h"
#include <stdint.h>
#include <arduino.h>

#define ZERO 0

typedef enum { hyst = 50, IDLE_INPUT = 500 } IdleRange;
typedef enum { SPEED_TWO_PAUSE 	= 100, SPEED_ONE_PAUSE 	= 550, IDLE_PAUSE = 1000 } DisplayPauses;
typedef enum { FLYWHEEL_IDLE_SPEED = 2, FLYWHEEL_SPEED_ONE 	= 4, FLYWHEEL_SPEED_TWO	= 8 } FlywheelSpeeds;

static const int BUTTON_WAIT = 1000;

uint8_t isIdle 	 = 0;
uint8_t engineON = 0;

uint8_t speedOneInput = 0;
uint8_t speedTwoInput = 0;

static unsigned long lastSwitch = ZERO;

void checkEngineON(void) {

	int now = millis();
	
	uint8_t z = readJoystick_Z();
	uint8_t toggle = z && (now - lastSwitch > BUTTON_WAIT);
	
	if (toggle) {
		lastSwitch = now;
		engineON = !engineON;
	}
}

void idleCheck(void) {
	
	int x = readJoystick_X();
	isIdle = engineON && (x < IDLE_INPUT + hyst) && (x > IDLE_INPUT - hyst);
}

void getSpeedInput(void) {
	
	int x = readJoystick_X();
	
	speedOneInput = x > IDLE_INPUT + hyst;
	speedTwoInput = x < IDLE_INPUT - hyst;
}

void checkEngineState(void) {
	checkEngineON();
	idleCheck();
	getSpeedInput();
}

int getFlywheelSpeed(void) {
	
	int newSpeed = ZERO;
		
	if (isIdle) 			{ newSpeed = FLYWHEEL_IDLE_SPEED;}
	else if (speedOneInput) { newSpeed = FLYWHEEL_SPEED_ONE;}
	else if (speedTwoInput)	{ newSpeed = FLYWHEEL_SPEED_TWO;}
	
	return newSpeed;
}

int getCylinderPause(void) {
	
	int newPause = IDLE_PAUSE;
	
	if (isIdle) 			{ newPause = IDLE_PAUSE;}
	else if (speedOneInput) { newPause = SPEED_ONE_PAUSE;}
	else if (speedTwoInput) { newPause = SPEED_TWO_PAUSE;}
	
	return newPause;
}

void driveEngine(void) {
	
	checkEngineState();
	
	if (engineON) {
		
		spinFlywheel(getFlywheelSpeed());
		
		displayCylinder(getCylinderPause());
	}
}

/**void checkEngineON(void) {

	int now = millis();
	
	uint8_t z = readJoystick_Z();
	uint8_t toggle = z && (now - lastSwitch > BUTTON_WAIT);
	
	if (toggle) {
		lastSwitch = now;
		engineON = !engineON;
	}
}

void idleCheck(void) {
	
	int x = readJoystick_X();
	isIdle = engineON && (x < IDLE_INPUT + hyst || x > IDLE_INPUT - hyst);
}

void getSpeedInput(void) {
	
	int x = readJoystick_X();
	
	speedOneInput = x > IDLE_INPUT + hyst;
	speedTwoInput = x < IDLE_INPUT - hyst;
	
	isIdle = !(speedOneInput || speedTwoInput);
}

void getServoSpeed(void) {
	
	getSpeedInput();
	
	if (isIdle) 		{ flywheelSpeed = SERVO_SPEED_IDLE;}
	if (speedOneInput) 	{ flywheelSpeed = SERVO_SPEED_ONE;}
	if (speedTwoInput)	{ flywheelSpeed = SERVO_SPEED_TWO;}
}

int getCylinderPause(void) {
	
	int newPause = IDLE_PAUSE;
	
	if (isIdle) 		{ newPause = IDLE_PAUSE;}
	if (speedOneInput) 	{ newPause = SPEED_ONE_PAUSE;}
	if (speedTwoInput) 	{ newPause = SPEED_TWO_PAUSE;}
	
	return newPause;
}

void driveEngine(void) {
	
	checkEngineON();

	idleCheck();
	
	if (engineON) {
		
		displayCylinder(getCylinderPause());
		
		getServoSpeed();
		
		moveServo();
	}
}**/
