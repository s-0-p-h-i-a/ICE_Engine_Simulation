#include "joystick.h"
#include "cylinder.h"
#include "cylinderVisuals.h"
//#include "servolib.h"
#include "drive.h"
#include <arduino.h>

#define ZERO 0

static const int hyst		= 50;
static const int IDLE_INPUT	= 500;
	
static const int SPEED_TWO_PAUSE 	= 100;
static const int SPEED_ONE_PAUSE 	= 550;
static const int IDLE_PAUSE 		= 1000;

//static const int SERVO_SPEED_ONE 	= 4;
//static const int SERVO_SPEED_TWO	= 8;

static const int BUTTON_WAIT = 1000;

bool isIdle 	 = false;
bool engineON = false;

bool speedOneInput = false;
bool speedTwoInput = false;

//static int servoSpeed = ZERO;

static unsigned long lastSwitch = ZERO;

void checkEngineON(void) {

	int now = millis();
	
	bool z = readJoystick_Z();
	bool toggle = z && (now - lastSwitch > BUTTON_WAIT);
	
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

/*int getServoSpeed(void) {
	
	int newSpeed = ZERO;
	
	if (isIdle) 		{ servoSpeed = ZERO;}
	if (speedOneInput) 	{ servoSpeed = SERVO_SPEED_ONE;}
	if (speedTwoInput)	{ servoSpeed = SERVO_SPEED_TWO;}
	
	return newSpeed;
}*/

int getCylinderPause(void) {
	
	int newPause = IDLE_PAUSE;
	
	if (isIdle) 				{ newPause = IDLE_PAUSE;}
	if (speedOneInput) 	{ newPause = SPEED_ONE_PAUSE;}
	if (speedTwoInput) 	{ newPause = SPEED_TWO_PAUSE;}
	
	return newPause;
}

void driveEngine(void) {
	
	checkEngineON();

	idleCheck();
	
	if (engineON) {
		
		getSpeedInput();

		runCylinder();

		displayCylinder(getCylinderPause());
		
		//moveServo(getServoSpeed());
	}
}
