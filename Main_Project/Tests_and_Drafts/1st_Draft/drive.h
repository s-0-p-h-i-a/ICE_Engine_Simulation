#ifndef DRIVE_H
#define DRIVE_H

enum {
	
	hyst		= 50,
	IDLE_INPUT	= 500
	//LEVEL_THREE_INPUT	= 550
};

enum {

	SPEED_THREE_PAUSE = 250,	// 60RPM x 100	
	SPEED_TWO_PAUSE = 500,	// 30RPM x 100
	SPEED_ONE_PAUSE = 1500, // 1RPM x 100
	IDLE_PAUSE 	= 2000,
};

enum {
	SPEED_ONE 	= 4,
	SPEED_TWO	= 8,
	SPEED_THREE = 12
};

enum { 
	IDLE_WAIT  = 100,
	WAIT_PAUSE = 1000
	//ISOLATE_X_Y = 150
	};

int getServoSpeed(void);
int getCylinderPause(void);
void checkEngineState(void);
void driveEngine(int pause);

#endif
