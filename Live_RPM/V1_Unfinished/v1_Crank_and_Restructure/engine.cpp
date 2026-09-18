#include "drive.h"
#include "crankshaft.h"
#include "engine.h"

//	========= CONSTANTS =========
// Engine ON sub-states
typedef enum { IDLE = 0, ROTATIONAL_SPEED_ONE = 1, ROTATIONAL_SPEED_TWO = 2 } RotationalSpeeds;

//	========= VARIABLES =========
static bool engineON = 0;
static RotationalSpeeds throttleState = IDLE;

//	========= FUNCTIONS =========
// Accessor functions for plotter display
bool getEngineON(void) 			{ return engineON; }
unsigned int getThrottleState	{ return throttleState; }

// MAIN DRIVE FUNCTION
// Full state update and control value assignment
void driveEngine(void) {
	
	// Engine state update
	engineON = turnEngineOnOff(engineON);
	
	if (engineON) {
		
		throttleState = assignThrottleLevel();
		
		moveCrankshaft(throttleState);
	}
}
