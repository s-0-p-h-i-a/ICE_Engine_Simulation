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

typedef enum {	CRANK_OK,
				CAM_INIT_ERROR,
				CRANK_ANGLE_UPDATE_ERROR,
				PISTON_UPDATE_ERROR,
				CAM_UPDATE_ERROR } CrankSystemStatuses;
				
typedef enum {	BDC_STATE,
				TDC_STATE } PistonStates;

// CONSTANTS
static const crank_timer TIMER_PHASE	= 50;
static const crank_timer TIMER_ZERO 	= 0;

// LOCAL VARIABLES
static angle crankshaftAngle 			= ANGLE_TDC;
static crank_timer lastCrankPollTime 	= TIMER_ZERO;
static PistonStates pistonState			= BDC_STATE;
static CrankSystemStatuses crankshaftSystemStatus = CRANK_OK;

// FUNCTIONS
// STATIC
static bool crankTimerPhaseElapsed(void);
static bool updatePistonState(void);
static bool pushPiston(uint8_t engineLoadNow);

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
	
	if ((crankshaftAngle > ANGLE_90) && (crankshaftAngle <= ANGLE_270)) {
		pistonState = BDC_STATE;
	}
	else {
		pistonState = TDC_STATE;
	}
	return 1;
}

// NON STATIC
// INIT
bool crankshaftInit(void) {
	crankshaftAngle 	= ANGLE_TDC;
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
	
	crankshaftAngle = updateAngle(crankshaftAngle + currentEngineLoad);
	
	if (!checkAngleValid(crankshaftAngle)) {
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
