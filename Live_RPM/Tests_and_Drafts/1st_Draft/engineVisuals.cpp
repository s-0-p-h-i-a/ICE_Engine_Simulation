#include "global.h"
#include "engine.h"
#include "engineVisuals.h"
#include <arduino.h>

static const bool pinMap[TOTAL_PINS] = { intakeValvePin, exhaustValvePin, fuelInjectorPin, sparkPlugPin, tdcPin1, tdcPin2, bdcPin1, bdcPin2 };

void pistonWrite(bool pistonUP) {
	
	if (pistonUP) {
		digitalWrite(tdcPin1, HIGH);
		digitalWrite(tdcPin2, HIGH);
		digitalWrite(bdcPin1, LOW);
		digitalWrite(bdcPin2, LOW);
	} else {
		digitalWrite(tdcPin1, LOW);
		digitalWrite(tdcPin2, LOW);
		digitalWrite(bdcPin1, HIGH);
		digitalWrite(bdcPin2, HIGH);
	}
}

void displayEngine(void) {
	
	for (int i = INTAKE_VALVE; i <= CYLINDER_ELEMENTS; ++i) {
			
		if (i = PISTON) { cylinder[i] ? pistonWrite(TDC) : pistonWrite(BDC);}
		
		else { cylinder[i] ? digitalWrite(pinMap[i], HIGH) : digitalWrite(pinMap[i], LOW);}
	}
}
