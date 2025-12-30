/*
RPM System Values for Serial Plotter Display

Function / Role:
- Defines signal processing values for RPM system data display
- Feeds processed values to plotter module

*/

#include "flywheel_lib.h"
#include "hall_lib.h"
#include "rpm_lib.h"

//	========= CONSTANTS =========
#define flywheelAngle_AMPLIFY 1
#define hallState_AMPLIFY 50
#define hallSecondPass_AMPLIFY 70
#define rpm_AMPLIFY 100

// 	========= VARIABLES =========
unsigned int plotter_FlywheelAngle;
bool plotter_HallState;
bool plotter_HallSecondPass;
double plotter_RPM;


// 	========= FUNCTIONS =========
void updateRpmSystemValues (void) {
	plotter_FlywheelAngle	= getFlywheelAngle() * flywheelAngle_AMPLIFY;
	plotter_HallState		= getHallState() * hallState_AMPLIFY;
	plotter_HallSecondPass	= getSecondPass() * hallSecondPass_AMPLIFY;
	plotter_RPM				= getRPM() * rpm_AMPLIFY;
}
