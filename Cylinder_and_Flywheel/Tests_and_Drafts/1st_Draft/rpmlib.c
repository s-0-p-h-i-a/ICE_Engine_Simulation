#include "global.h"
#include "engine.h"
#include "hall_lib.h"
#include "rpmlib.h"
#include <stdbool.h>
#include <math.h>

int RPM = 0;
static int thisRPM = 0;
static int lastRPM = 0;

void getTimeWindow(void) {
	
	if (hallON) {

		timeWindow = currentSensorRead - lastSensorRead;		
	} else {

		timeWindow = now - lastSensorRead;	
	}
}

void getRPM(void) {
	
	checkReadState();
	
	if (isIdle) { RPM = ZERO;}

	if (hallON && secondPass) { ;
	} else {
		lastRPM = thisRPM;			
		thisRPM = round(timeWindow/60000.0);	// just in case to avoid div by 0
	}
	RPM = round((thisRPM + lastRPM) / 2.0);
}
