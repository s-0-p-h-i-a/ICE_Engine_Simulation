/*
RPM Calculation System:
RPM Math + Return Module

Function / Role:
- Calculate RPM from Hall sensor trigger timing

Process:
 - Update internal state for simple smoothing
 - Return the current RPM as a double
 
System layers (top -> bottom):
flywheel_ref_lib.cpp
servo_lib.cpp
hall_lib.cpp
rpm_lib.cpp

*/

#include "hall_lib.h"
#include "rpmlib.h"

//	========= CONSTANTS =========
static const double MS_PER_MINUTE	= 60000.0;
static const double AVERAGE_FACTOR	= 2.0;

//	========= VARIABLES =========
static double prevRPM 		= 0.0;
static double currentRPM	= 0.0;

//	========= FUNCTION =========
// Accessor for plotter display
double getRPM(void) { return currentRPM; }

// Computes RPM from Hall sensor timing
// Updates internal currentRPM with simple 2-step smoothing
// Returns the smoothed RPM as a double
double computeRPM(void) {
	
	updateHallState();
	
	prevRPM = currentRPM;			
	currentRPM = MS_PER_MINUTE / computeTimeWindow();
	
	// Simple 2-step smoothing to reduce display jitter
	double smoothedRPM = (currentRPM + prevRPM) / AVERAGE_FACTOR;
	currentRPM = smoothedRPM;
	
	return smoothedRPM;
}
