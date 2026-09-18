#include "crank_piston.h"
#include "angles.h"
#include "camshaft.h"
#include <stdint.h>
#include <stdbool.h>

// ENUMS / DEFINES / CONSTANTS
typedef uint8_t crank_ticks;

typedef enum {	CRANK_OK,
				CRANK_ANGLE_UPDATE_ERROR,
				PISTON_UPDATE_ERROR } CrankSystemStatuses;
				
typedef enum {	BDC,
				TDC,
				PISTON_BETWEEN_STATES } PistonStates;

static const uint8_t TICKER_PHASE_LENGTH	= 10;
static const uint8_t TICKER_PHASE_ZERO		= 0;

static const angle_position TDC_ANGLE 	= 180;
static const angle_position BDC_ANGLE 	= 0;

static const crank_ticks MAX_TICKS 	= 250;
static const crank_ticks TEN_TICKS 	= 10;
static const crank_ticks ZERO_TICKS = 0;

static const uint8_t ZERO_LOAD = 0;
static const bool TDC = 1;
static const bool BDC = 0;

// LOCAL VARIABLES
static angle_position crankshaftAngle 	= BDC_POSITION;
static crank_ticks crankSystemTicker 	= ZERO_TICKS;
static bool tickerPhaseElapsed			= 0;
static PistonStates pistonState			= BDC;

static CrankSystemStatuses crankshaftSystemStatus = CRANK_OK;

// FUNCTIONS
//// FORWARD DECLARATIONS
static bool updateCrankSystemTicks(void);
static bool checkTickerPhaseElapsed(void);
static bool wrapCrankshaftAngle(void);
static bool updateCrankAngle(void);

//// STATIC FUNCTIONS
static crank_ticks updateCrankSystemTicks(crank_ticks currentTicks) {

	if (currentTicks == MAX_TICKS) {
		currentTicks = ZERO_TICKS;
	}
	
	if (currentTicks > MAX_TICKS) {
		currentTicks = currentTicks - MAX_TICKS;
	}
	
	++currentTicks;
	return currentTicks;
}

static bool checkTickerPhaseElapsed(void) {
	
	return (crankSystemTicker % TICKER_PHASE_LENGTH) == TICKER_PHASE_ZERO;
}

static PistonStates updatePistonState(angle_position newAngle) {
	
	if (newAngle == TDC_ANGLE) {
		return TDC;
	}
	if (newAngle == BDC_ANGLE) {
		return BDC;
	}
	return PISTON_BETWEEN_STATES;
}

static bool pistonEdgeState(void) {
	
	if ((pistonState == BDC) || (pistonState == TDC)) {
		return 1;
	}
	return 0;
}

static bool pushPiston(uint8_t engineLoadNow) {
	
	if (!checkTickerPhaseElapsed()) {
		return 1;
	}
	
	crankshaftAngle = updateAngle(crankshaftAngle + engineLoadNow);
	
	if (!checkAngleValid(crankshaftAngle)) {
		crankshaftSystemStatus = CRANK_ANGLE_UPDATE_ERROR;
		return 0;
	}
	
	pistonState = updatePistonState(crankshaftAngle);
	
	if (!checkPistonStateValid(pistonState)) {
		crankshaftSystemStatus = PISTON_UPDATE_ERROR;
		return 0;
	}
		
	if (pistonEdgeState()) {
		updateCylinder(pistonState);
	}
	return 1;
}

//// INIT
bool crankshaftInit(void) {
	crankshaftAngle 	= BDC_POSITION;
	pistonState			= BDC;
	crankSystemTicker 	= ZERO_TICKS;
	tickerPhaseElapsed	= 0;

	return 1;
}

// MAIN FUNCTION
bool updateCrank(uint8_t currentEngineLoad) {
	
	crankSystemTicker = updateCrankSystemTicks(crankSystemTicker);
	
	if (currentEngineLoad > ZERO_LOAD) {
		
		pushPiston(currentEngineLoad);
		updateCamshaft(clampAndCorrect(crankshaftAngle));
	}
	return 1;
}
