#include "flywheel_ref.h"
#include "cylinderVisuals.h"
#include "crankshaftVisuals.h"
#include "crankshaft.h"

//	========= CONSTANTS =========
#define CRANKSHAFT 0
#define LEDS 1
#define TOTAL_ELEMENTS 2
#define TOTAL_STATES 3
// Angles
enum {
	ANGLE_ZERO 	= 0,
	ANGLE_360 	= 360
};

// CONTROL CONSTANTS:
// Crankshaft angle increment steps
typedef enum { IDLE_STEP = 2, SPEED_ONE_STEP = 4, SPEED_TWO_STEP = 8 } CrankshaftAngleSteps;

// CONTROL ARRAYS:
// Crankshaft steps array
static const unsigned int crankshaftAngleSteps[TOTAL_STATES] = { IDLE_STEP, SPEED_ONE_STEP, SPEED_TWO_STEP };

// 	========= VARIABLES =========
// Current crankshaft position
static unsigned int crankshaftAngle = ANGLE_ZERO;

//	========= FUNCTIONS =========
// Helper functions
static void wrapCrankshaftAngle(void);
static unsigned int assignCrankshaftAngleStep(unsigned int throttleInput);
static unsigned int assignLEDBlinkPause(unsigned int throttleInput);
static void advanceCrankshaftAngle(unsigned int crankshaftAngleStep);

// Accessor for plotter display
unsigned int getCrankshaftAngle(void) { return crankshaftAngle; }

// Wraps crankshaftAngle into [0°, 360°) domain
static void wrapCrankshaftAngle(void) {
		
	if (crankshaftAngle > ANGLE_360) {
		crankshaftAngle = crankshaftAngle % ANGLE_360;
	}	
}

// Set crankshaft angle increment
static unsigned int assignCrankshaftAngleStep(unsigned int throttleInput) {
	
	return crankshaftAngleSteps[throttleInput];
}

// Stepwise movement via user-defined speed input
static void advanceCrankshaftAngle(unsigned int crankshaftAngleStep) {
	
	crankshaftAngle += crankshaftAngleStep;
	wrapCrankshaftAngle();
	
	advanceFlywheelAngle(crankshaftAngle);
}

void moveCrankshaft(unsigned int throttleInput) {
	
	advanceCrankshaftAngle(assignCrankshaftAngleStep(throttleInput));
	
	advanceCrankshaft(throttleInput);	
}
