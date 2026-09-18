/**
Crankshaft and piston:

- Updates crankshaft angle based on engine load: new angle = old angle + engine load value
- Updates piston state based on angle range
- Calls update to camshaft using crank angle as argument

**/

#include "crank_piston.h"
#include "crank_piston_anglegetter.h"
#include "../lib/angles.h"
#include "camshaft.h"
#include <stdint.h>
#include <stdbool.h>

// DEFINITIONS
typedef unsigned long crank_timer;
typedef uint8_t angle_increment;

typedef enum {	CRANK_OK,
				CAM_INIT_ERROR,
				CRANK_ANGLE_UPDATE_ERROR,
				PISTON_UPDATE_ERROR,
				CAM_UPDATE_ERROR } CrankSystemStatuses;
				
typedef enum {	BDC_STATE,
				TDC_STATE,
				BDC_TO_TDC_STATE,
				TDC_TO_BDC_STATE } PistonStates;

// CONSTANTS
static const crank_timer TIMER_PHASE	= 100;
static const crank_timer TIMER_ZERO 	= 0;
static const uint8_t LOAD_TO_INCREMENT_RATIO = 10;
static const angle_increment INCREMENT_ZERO = 0;
static const angle_increment MAX_INCREMENT	= 10;

// LOCAL VARIABLES
static angle crankshaftAngleNow			= ANGLE_TDC;
static angle lastCrankshaftAngle 			= ANGLE_TDC;
static angle_increment currentIncrement	= INCREMENT_ZERO;
static crank_timer lastCrankPollTime 	= TIMER_ZERO;
static PistonStates pistonState			= BDC_STATE;
static CrankSystemStatuses crankshaftSystemStatus = CRANK_OK;

// FUNCTIONS
// STATIC
static bool crankTimerPhaseElapsed(void);
static bool updatePistonState(void);
static bool pushPiston(uint8_t engineLoadNow);
static angle_increment clampIncrement(angle_increment increment)

// CHECK PHASE ELAPSED
static bool crankTimerPhaseElapsed(void) {
	
	unsigned long timeNow = HAL_GetTick();
	unsigned long delta = timeNow - lastCrankPollTime;
	
	bool phaseElapsed = delta >= TIMER_PHASE;
	
	if (phaseElapsed) {
		lastPollTime = timeNow;
	}
	return phaseElapsed;
}

// UPDATE PISTON
static bool updatePistonState(void) {
	
	switch (pistonState) {
		
		case TDC: {
			pistonState = TDC_TO_BDC_STATE;
		} break;
		
		case BDC: {
			pistonState = BDC_TO_TDC_STATE;
		} break;
		
		case TDC_TO_BDC_STATE: {
			if ((lastCrankshaftAngle < ANGLE_BDC) && (crankshaftAngleNow >= ANGLE_BDC)) {
				pistonState = BDC;
			}
		} break;
		
		case BDC_TO_TDC_STATE:  {
			if (crankshaftAngleNow < lastCrankshaftAngle) {
				pistonState = TDC;
			}
		} break;
		
		default: {
			return 0;
		}
	}
	return 1;
}

// CLAMP ANGLE INCREMENT
static angle_increment clampIncrement(angle_increment increment) {
	
	if (increment > MAX_INCREMENT) {
		increment = MAX_INCREMENT;
	}
	return increment;
}

// UPDATE ANGLE
static bool updateCrankAngle(void) {
	
	currentIncrement = currentEngineLoad / LOAD_TO_INCREMENT_RATIO;
	lastCrankshaftAngle = crankshaftAngleNow;
	crankshaftAngleNow = updateAngle(crankshaftAngle + clampIncrement(currentIncrement));
	
	if (!checkAngleValid(crankshaftAngleNow)) {
		return 0;
	}
	return 1;
}

// NON STATIC
// INIT
bool crankshaftInit(void) {
	crankshaftAngle 	= ANGLE_TDC;
	currentIncrement	= INCREMENT_ZERO;
	pistonState			= TDC_STATE;
	crankSystemTicker 	= ZERO_TICKS;
	tickerPhaseElapsed	= 0;
	
	if (!camshaftInit()) {
		crankshaftSystemStatus = CAM_INIT_ERROR;
		return 0;
	}
	return 1;
}

// UPDATE
bool updateCrank(uint8_t currentEngineLoad) {
	
	if (!crankTimerPhaseElapsed()) {
		crankshaftSystemStatus = CRANK_OK;
		return 1;
	}
	
	if (!updateCrankAngle()) {
		crankshaftSystemStatus = CRANK_ANGLE_UPDATE_ERROR;
		return 0;
	}
		
	if (!updatePistonState()) {
		crankshaftSystemStatus = PISTON_UPDATE_ERROR;
		return 0;
	}
	
	if (!updateCamshaft(crankshaftAngle)) {
		crankshaftSystemStatus = CAM_UPDATE_ERROR;
		return 0;
	}
	
	crankshaftSystemStatus = CRANK_OK;
	return 1;
}

// GETTER
angle getCrankshaftAngle(void) {
	return crankshaftAngle;
}
