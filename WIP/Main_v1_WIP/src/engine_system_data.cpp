/*
Engine System Values for Serial Plotter Display

Function / Role:
- Defines signal processing values for engine+cylinder system data display
- Feeds processed values to plotter module

*/

#include "drive.h"
#include "cylinder.h"

//	========= CONSTANTS =========
#define engineON_AMPLIFY 100
#define engineON_SHIFT 50
#define isIdle_AMPLIFY 100
#define isIdle_SHIFT 70
#define speedOne_AMPLIFY 50
#define speedOne_SHIFT 20
#define speedTwo_AMPLIFY 50
#define speedTwo_SHIFT 20
#define cylinderState_AMPLIFY 100
#define cylinderState_SHIFT 30

// 	========= VARIABLES =========
bool plotter_engineON;
bool plotter_isIdle;
bool plotter_speedOne;
bool plotter_speedTwo;
bool plotter_cylinderState;


// 	========= FUNCTIONS =========
void updateEngineSystemValues (void) {
	plotter_engineON		= getEngineON() * engineON_AMPLIFY + engineON_SHIFT;
	plotter_isIdle			= getIsIdle() * isIdle_AMPLIFY + isIdle_SHIFT;			
	plotter_speedOne		= getSpeedOneInput() * speedOne_AMPLIFY + speedOne_SHIFT;
	plotter_speedTwo		= getSpeedTwoInput() * speedTwo_AMPLIFY + speedTwo_SHIFT;
	plotter_cylinderState	= getCurrentCylinderState()	* cylinderState_AMPLIFY + cylinderState_SHIFT;
}
