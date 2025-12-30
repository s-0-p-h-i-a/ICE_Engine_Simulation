#include "global.h"
#include "joystick.h"
#include "engine.h"
#include "drive.h"
#include <arduino.h>

static int speedPause = IDLE_PAUSE;
static long unsigned int lastEngineCycle = ZERO;

static bool speedOneInput = false;
static bool speedTwoInput = false;

static int speed = ZERO;


void getSpeedInput(void) {
	
	speedOneInput = x > IDLE_INPUT + hyst;
	speedTwoInput = x < IDLE_INPUT - hyst;
}

int getServoSpeed(void) {
	
	int newSpeed = ZERO;
	
	if (isIdle) 	{ speed = ZERO;}
	
	if (speedOneInput) { speed = SPEED_ONE;}
	
	if (speedTwoInput) { speed = SPEED_TWO;}
	
	return newSpeed;
}

int getCylinderPause(void) {
	
	int newPause = IDLE_PAUSE;
	
	if (isIdle) 	{ speedPause = IDLE_PAUSE;}
	
	if (speedOneInput) { speedPause = SPEED_ONE_PAUSE;}
	
	if (speedTwoInput) { speedPause = SPEED_TWO_PAUSE;}
	
	//if (x > LEVEL_TWO_INPUT && x < LEVEL_THREE_INPUT - hyst) { speedPause = SPEED_THREE_PAUSE;}
	
	return newPause;
}

void checkEngineState(void) {
	
	// Idle check
	bool idleInputX = x > IDLE_INPUT - hyst && x < IDLE_INPUT + hyst;
	
	bool idleTimeX = now - lastUpdate_X > IDLE_WAIT;
	
	isIdle = idleInputX && idleTimeX;
	
	// ON/OFF toggle
	if (z && now - lastUpdate_Z > WAIT_PAUSE) {
		engineON 		= !engineON;
	}
	
	// LED + servo control updates
	if (!isIdle) {
		speed = getServoSpeed();
		speedPause = getCylinderPause();
	}
}

void driveEngine(void) {
	
	runEngine();
	
	while (millis() < now + speedPause) {;}
}
