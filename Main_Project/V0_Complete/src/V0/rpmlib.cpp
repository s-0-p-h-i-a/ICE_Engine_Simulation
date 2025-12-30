#include "hall_lib.h"
#include "rpmlib.h"
#include <stdint.h>
#include <math.h>

#define ZERO 0
static const double MINUTE = 60000.0;

static double thisRPM = ZERO;
static double lastRPM = ZERO;

double getRPM(void) {
	
	updateHallState();
	
	lastRPM = thisRPM;			
	thisRPM = MINUTE / getTimeWindow();

	double RPM = (thisRPM + lastRPM) / 2.0;
	thisRPM = RPM;
	
	return RPM;
}
