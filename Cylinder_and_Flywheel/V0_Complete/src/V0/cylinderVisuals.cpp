#include "cylinder.h"
#include "cylinderVisuals.h"
#include <stdint.h>
#include <arduino.h>


//// CONSTANTS
#define ZERO 0

enum {
	bdcPin2           = 6,
	tdcPin2           = 7,
	sparkPlugPin      = 8,
	exhaustValvePin   = 9,
	bdcPin1           = 10,
	tdcPin1           = 11,
	fuelInjectorPin   = 12,
	intakeValvePin    = 13 
};

typedef enum { ONE = 1, PISTON_PINS = 2, HEADPINS = 4 } PinQuantities; 

typedef enum { HEAD_ELEMENTS = 3, PISTON = 4 } LoopFlags;

static const int cylinderPinMap[HEADPINS] = { intakeValvePin, exhaustValvePin, fuelInjectorPin, sparkPlugPin };
static const int pistonPinsTDC[PISTON_PINS] = { tdcPin1, tdcPin2 };
static const int pistonPinsBDC[PISTON_PINS] = { bdcPin1, bdcPin2 };
static const uint8_t TDC = 1;
static const uint8_t BDC = 0;

static uint8_t switchNow = 1;
static uint8_t displayON = 1;
static unsigned long lastSwitch = ZERO;

void startEngineVisuals(void) {
	for (int i = 0; i < HEADPINS; ++i) {
		pinMode(cylinderPinMap[i],OUTPUT);
	}
	pinMode(tdcPin1,OUTPUT);
	pinMode(tdcPin2,OUTPUT);
	
	pinMode(bdcPin1,OUTPUT);
	pinMode(bdcPin2,OUTPUT);
}

void pistonWrite(uint8_t pistonUP) {
	
	if (pistonUP) {
		digitalWrite(tdcPin1, HIGH);
		digitalWrite(tdcPin2, HIGH);
	} else {
		digitalWrite(bdcPin1, HIGH);
		digitalWrite(bdcPin2, HIGH);
	}
}

void writePins(void) {

	cylinderElementStates[PISTON] ? pistonWrite(TDC) : pistonWrite(BDC);

	for (int i = ZERO; i < PISTON; ++i) {
		digitalWrite(cylinderPinMap[i], cylinderElementStates[i]);
	}
}

void clearDisplay(void) {

	for (int j = ZERO; j < PISTON_PINS; ++j) {
		digitalWrite(pistonPinsTDC[j], LOW);
		digitalWrite(pistonPinsBDC[j], LOW);
	}
	
	for (int i = ZERO; i < PISTON; ++i) {
		digitalWrite(cylinderPinMap[i], LOW);
	}
}

void displayCylinder(int pause) {

	unsigned long currentTime = millis();
	unsigned long delta = currentTime - lastSwitch;

	if (delta > pause) {
			
		switchNow = 1;
		displayON = !displayON;
		lastSwitch = currentTime;
	}
	
	else { switchNow = 0; }
		
	if (switchNow) {
		if (displayON) { clearDisplay(); updateCylinderState(); }
		else { writePins(); }	
	}	
}

	
