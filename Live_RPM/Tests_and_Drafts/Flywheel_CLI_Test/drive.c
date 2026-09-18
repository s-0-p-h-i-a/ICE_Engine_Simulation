#include "flywheel_libTEST.h"
#include "drive.h"
#include <stdbool.h>
#include <stdio.h>

#define ZERO 0
#define MAX_LOOPS 200

typedef enum { hyst = 50, IDLE_INPUT = 500 } IdleRange;
typedef enum { FLYWHEEL_SPEED_IDLE = 2, FLYWHEEL_SPEED_ONE = 4, FLYWHEEL_SPEED_TWO = 8 } FlywheelSpeeds;
typedef enum { SPEED_IDLE = 1, SPEED_ONE = 2, SPEED_TWO = 3 } InputSpeeds; 

static bool isIdle 	 = 0;
static bool engineON = 0;
static bool speedOneInput = 0;
static bool speedTwoInput = 0;

int currentLoop = ZERO;

void checkEngineON(void) {
	
	int input = ZERO;
	printf("Engine ON/OFF? 1/0 \n");
	scanf("%d", &input);
	engineON = input;
	isIdle = engineON;
}

void getSpeedInput(void) {
	
	int speedInput = ZERO;
	
	printf("Speed level? 1(idle)-2-3 \n");
	scanf("%d", &speedInput);
	
	isIdle			= speedInput == SPEED_IDLE;
	speedOneInput 	= speedInput == SPEED_ONE;
	speedTwoInput 	= speedInput == SPEED_TWO;
}

bool getEngineStateCheckNow(void) {
	
	bool checkStateNow = currentLoop == MAX_LOOPS;
	return checkStateNow;
}

void checkEngineState(void) {
	checkEngineON();
	getSpeedInput();
}

int getFlywheelSpeed(void) {
	
	int newSpeed = ZERO;
		
	if (isIdle) 		{ newSpeed = FLYWHEEL_SPEED_IDLE;}
	if (speedOneInput) 	{ newSpeed = FLYWHEEL_SPEED_ONE;}
	if (speedTwoInput)	{ newSpeed = FLYWHEEL_SPEED_TWO;}
	
	return newSpeed;
}

void driveEngine(void) {
		
	if (engineON) { spinFlywheel(getFlywheelSpeed()); }
}
