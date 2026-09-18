/**
Crankshaft and piston:

- Updates camshaft angle based on crankshaft angle: cam angle = crank angle / 2
- Calls valves update if edge angle has been passed
- Owns stroke state

**/

#include "../lib/strokes.h"
#include "../lib/angles.h"
#include "camshaft.h"
#include "camshaft_strokegetter.h"
#include "cylinder_head_src/valve_control.h"
#include <stdint.h>
#include <stdbool.h>

// DEFINITIONS
typedef enum {	CAM_OK,
				VALVES_INIT_ERROR,
				VALVES_UPDATE_ERROR } CamSystemStatuses;
				
// CONSTANTS				
static const size_t TOTAL_VALVES = 2;
static const size_t TOTAL_STATES = 2;
static const uint8_t GEAR_RATIO = 2;

static const angle intakeValveOpenOffset	= 20;
static const angle intakeValveCloseOffset	= 50;

static const angle intakeValveOpenRangeStart = ANGLE_TDC - intakeValveOpenOffset;
static const angle intakeValveOpenRangeEnd	 = ANGLE_BDC + intakeValveCloseOffset;

static const angle exhaustValveOpenOffset	= 50;
static const angle exhaustValveCloseOffset	= 20;

static const angle exhaustValveOpenRangeStart = ANGLE_BDC - exhaustValveOpenOffset;
static const angle exhaustValveOpenRangeEnd	  = ANGLE_TDC + exhaustValveCloseOffset;

static const angle valveRangeLimits[TOTAL_STROKES] = { exhaustValveOpenRangeStart, , exhaustValveOpenRangeStart, intakeValveOpenRangeStart };

// VARIABLES
static angle camshaftAngleThisLoop = ANGLE_TDC;
static angle camshaftAngleLastLoop = ANGLE_TDC;
static CamSystemStatuses camshaftSystemStatus = CAM_OK;
static Strokes currentStroke = INTAKE;

// FUNCTIONS
// STATIC
static bool newHalfRevComplete(void);
static bool toggleValveNow(void);

// NEW REVOLUTION
static bool newHalfRevComplete(void) {
	
	// new revolution = cam angle passed over the 0° 'reset' point
	bool halfRev = (camshaftAngleLastLoop < ANGLE_BDC) && (camshaftAngleThisLoop >= ANGLE_BDC);
	bool fullRev = camshaftAngleThisLoop < camshaftAngleLastLoop;
	halfRevCompleted = halfRev || fullRev;
	return newRev;
}

// TOGGLE VALVE
static bool toggleValveNow(void) {
	
	bool lastAngleOutOfRange = camshaftAngleLastLoop < valveRangeLimits[currentStroke];
	bool currentAngleInRange = camshaftAngleThisLoop >= valveRangeLimits[currentStroke];
	bool toggleNow = lastAngleOutOfRange && currentAngleInRange;
	return toggleNow;
}

// NON STATIC
// INIT
bool camshaftInit(void) {
	
	camshaftAngleThisLoop = ANGLE_TDC;
	camshaftAngleLastLoop = ANGLE_TDC;
	camshaftSystemStatus = CAM_OK;
	currentStroke = INTAKE;
	
	if (!valvesInit()) {
		camshaftSystemStatus = VALVES_INIT_ERROR;
		return 0;
	}
	return 1;
}
		
// UPDATE
bool updateCamshaft(angle crankAngle) {
	
	camshaftSystemStatus = CAM_OK;
	
	camshaftAngleLastLoop = camshaftAngleThisLoop;
	camshaftAngleThisLoop = crankAngle / GEAR_RATIO;
		
	if (newHalfRevComplete()) {
		currentStroke = advanceStroke(currentStroke);
	}
	
	if (toggleValveNow()) {
		if (!updateValves(currentStroke)) {
			camshaftSystemStatus = VALVE_UPDATE_ERROR;
			return 0;
		}
	}
	return 1;
}

// GETTERS
// ANGLE
angle getCamshaftAngle(void) {
	return camshaftAngleThisLoop;
}

// STROKE
Strokes getCurrentStroke(void) {
	return currentStroke;
}
