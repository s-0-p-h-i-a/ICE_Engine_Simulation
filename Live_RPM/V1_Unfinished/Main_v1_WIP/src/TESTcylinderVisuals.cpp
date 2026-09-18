/*
Cylinder System:

Function / Role:
- Blink LEDs representing cylinder elements on/off
- Move cylinder object forward in 4-stroke cycle when a full blink cycle (on+off) has passed
- Drives 2-phase blink cycle:
	- LEDs turned ON according to current stroke
	- LEDS turned OFF, stroke updated

Process:
- Helper functions turn LEDs on and off
- Timestamp + internal state variable + control flag help move LED display through blink cycle
- cylinderElementStates controls which LEDs are turned on this cycle
- displayCylinder calls updateCylinderState at the end of a full blink cycle
	-> cylinderElementStates moves forward in 4-stroke cycle
	= Updated LED instructions are ready for next blink cycle

Used by:
driveEngine in drive.cpp

Drives:
Cylinder state via updateCylinderState call

*/

#include "cylinder.h"
#include "cylinderVisuals.h"
#include <arduino.h>


//	========= CONSTANTS =========
// For-loop limits
typedef enum { HEAD_ELEMENTS = 3, PISTON = 4 } LoopLimits;

// LED pins
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
// Pin Quantities
typedef enum { PISTON_PINS = 2, HEADPINS = 4 } PinQuantities; 

// Cylinder head pins map
static const int cylinderPinMap[HEADPINS] = { intakeValvePin, exhaustValvePin, fuelInjectorPin, sparkPlugPin };
// Piston pins maps
static const int pistonPinsTDC[PISTON_PINS] = { tdcPin1, tdcPin2 };
static const int pistonPinsBDC[PISTON_PINS] = { bdcPin1, bdcPin2 };
// Piston states
static const bool TDC = 1;
static const bool BDC = 0;

// 	========= VARIABLES =========
// Control flags
static bool switchNow = 1;
static bool displayON = 1;
// Blink cycle timestamp
static unsigned long lastSwitch = 0;

//	========= FUNCTIONS =========
// Helper functions
static void pistonWrite(bool pistonUP);
static void writePins(void);
static void clearDisplay(void);

// Pin LEDs
void startEngineVisuals(void) {
	for (int i = 0; i < HEADPINS; ++i) {
		pinMode(cylinderPinMap[i],OUTPUT);
	}
	pinMode(tdcPin1,OUTPUT);
	pinMode(tdcPin2,OUTPUT);
	
	pinMode(bdcPin1,OUTPUT);
	pinMode(bdcPin2,OUTPUT);
}

// Control piston LEDs
static void pistonWrite(bool pistonUP) {
	
	if (pistonUP) {
		digitalWrite(tdcPin1, HIGH);
		digitalWrite(tdcPin2, HIGH);
	} else {
		digitalWrite(bdcPin1, HIGH);
		digitalWrite(bdcPin2, HIGH);
	}
}

// Control cylinder head LEDs
static void writePins(void) {

	cylinderElementStates[PISTON] ? pistonWrite(TDC) : pistonWrite(BDC);

	for (int i = ZERO; i < PISTON; ++i) {
		digitalWrite(cylinderPinMap[i], cylinderElementStates[i]);
	}
}

// Turn off all LEDs
static void clearDisplay(void) {

	for (int j = 0; j < PISTON_PINS; ++j) {
		digitalWrite(pistonPinsTDC[j], LOW);
		digitalWrite(pistonPinsBDC[j], LOW);
	}
	
	for (int i = 0; i < PISTON; ++i) {
		digitalWrite(cylinderPinMap[i], LOW);
	}
}

// Blink cycle + cylinder state update driver
void displayCylinder(unsigned int pause) {

	// Timestamp and delta
	unsigned long currentTime = millis();
	unsigned long delta = currentTime - lastSwitch;

	// Update control flags and timestamp
	if (delta > pause) {
			
		switchNow = 1;
		displayON = !displayON;
		lastSwitch = currentTime;
	} else {
		switchNow = 0;
	}
	
	// Control display + move cylinder through stroke cycle	
	if (switchNow) {
		// LEDs were on = blink cycle complete -> turn LEDs off, move cylinder forward
		if (displayON) { clearDisplay(); updateCylinderState(); }
		// LEDs were off -> display cylinder at new stroke
		else { writePins(); }	
	}	
}
