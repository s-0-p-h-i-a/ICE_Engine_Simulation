#include "cylinder.h"
#include "cylinderVisuals.h"
#include <arduino.h>

#define ZERO 0
#define PISTON_PINS 2
#define HEAD_ELEMENTS 3
#define HEADPINS_or_PISTONPOS 4

static const int cylinderPinMap[HEADPINS_or_PISTONPOS] = { intakeValvePin, exhaustValvePin, fuelInjectorPin, sparkPlugPin };
static const int pistonPinsTDC[PISTON_PINS] = { tdcPin1, tdcPin2 };
static const int pistonPinsBDC[PISTON_PINS] = { bdcPin1, bdcPin2 };
static const bool TDC = true;
static const bool BDC = false;

static bool switchNow = true;
static bool displayON = true;
static unsigned long lastSwitch = ZERO;

void pistonWrite(bool pistonUP) {

	if (pistonUP) {

		for (int j = ZERO; j < PISTON_PINS; ++j) {
		digitalWrite(pistonPinsTDC[j], HIGH); }
	} else {
	
		for (int j = ZERO; j < PISTON_PINS; ++j) {
			digitalWrite(pistonPinsBDC[j], HIGH);
			}
	}
}

void writePins(void) {

	cylinderElementStates[HEADPINS_or_PISTONPOS] ? pistonWrite(TDC) : pistonWrite(BDC);

	for (int i = ZERO; i < HEADPINS_or_PISTONPOS; ++i) {
		if (cylinderElementStates[i]) { digitalWrite(cylinderPinMap[i], HIGH); }
	}
}

void clearDisplay(void) {

	for (int j = ZERO; j < PISTON_PINS; ++j) {
		digitalWrite(pistonPinsTDC[j], LOW);
		digitalWrite(pistonPinsBDC[j], LOW);
	}
	
	for (int i = ZERO; i <= HEAD_ELEMENTS; ++i) {
		digitalWrite(cylinderPinMap[i], LOW);
	}
}

void displayCylinder(int pause) {

	unsigned long currentTime = millis();
	unsigned long delta = currentTime - lastSwitch;

	if (delta > pause) {
		switchNow = true;
		displayON = !displayON;
		lastSwitch = currentTime;
	}

	if (switchNow) {
		if (displayON) { clearDisplay(); switchCylinderState = true;}
		else { writePins(); switchCylinderState = false; }
	}
}

	
