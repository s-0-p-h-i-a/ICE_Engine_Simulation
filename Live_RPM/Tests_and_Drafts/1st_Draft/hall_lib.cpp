#include "global.h"
#include "hall_lib.h"
#include <Arduino.h>

bool hallON = false;
bool secondPass = false;

long unsigned int currentSensorRead = ZERO;
long unsigned int lastSensorRead = ZERO;

void initHall(void) {
	
	currentSensorRead = ZERO;
	lastSensorRead = ZERO;
	secondPass = false;
	
	pinMode(HALL_PIN, INPUT);
}

void checkReadState(void) {

	hallON = !digitalRead(HALL_PIN);
	
	if (hallON) {

		lastSensorRead = currentSensorRead;
		currentSensorRead = now;

		if (!secondPass) {	
			secondPass = true;		
		} else {
			secondPass = false;
		}
	}
}
