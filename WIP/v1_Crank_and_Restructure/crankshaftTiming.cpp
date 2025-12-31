#include "cylinder.h"
#include "cylinderVisuals.h"
#include "crankshaftVisuals.h"

//	========= CONSTANTS =========
#define CRANKSHAFT 0
#define LEDS 1
#define TOTAL_ELEMENTS 2
#define TOTAL_STATES 4

// 4-Stroke Cycle
typedef enum { INTAKE = 0, COMPRESSION = 1, COMBUSTION = 2, EXHAUST = 3 } Strokes;
// Cylinder LED display timing levels
typedef enum { SPEED_TWO_PAUSE = 100, SPEED_ONE_PAUSE = 550, IDLE_PAUSE = 1000 } CylinderDisplayPause;

// CONTROL ARRAYS:
// Blink pauses array
static const unsigned int LED_BlinkLevels[TOTAL_STATES] = { IDLE_PAUSE, SPEED_ONE_PAUSE, SPEED_TWO_PAUSE };

// 	========= VARIABLES =========
// Current stroke
static unsigned int currentStroke = INTAKE;

static bool displayON = 0;
static bool switchNow = 0;

// 	========= FUNCTIONS ========= 
// Helper functions
static unsigned int assignLEDBlinkPause(unsigned int throttleInput);
static void updateStroke(void);
static void updateTimingStates(unsigned int pause);
static void updateDisplay(void);

// Accessor for plotter display
Strokes getCurrentStroke(void) { return currentStroke; }

// Set cylinder LED display blink pause
static unsigned int assignLEDBlinkPause(unsigned int throttleInput) {

	return LED_BlinkLevels[throttleInput];
}

// Move forward in 4-stroke cycle
static unsigned int updateStroke(void) {
	if (currentStroke == EXHAUST) {
		return INTAKE;
		} else { 
			++currentStroke;
			return currentStroke;
		}
}

// Blink cycle + state update driver
static bool updateTimingStates(unsigned int pause) {

	// Timestamp and delta
	unsigned long currentTime = millis();
	unsigned long delta = currentTime - lastSwitch;

	// Update control flags and timestamp
	if (delta >= pause) {
		lastSwitch += pause;
		return 1;
	} else {
		return 0;
	}
}

static void endDisplayCycle(void) {
	displayON = 0;
	clearCrankshaftDisplay();
	clearCylinderDisplay();
	currentStroke = updateStroke();
	updateCylinderElements(currentStroke);
}

static void startDisplayCycle(void) {
	displayON = 1;
	writeCrankshaftPins(currentStroke);
	writeCylinderPins(currentStroke);
}

static void updateSystem(unsigned int pause) {
	
	bool switchNow = updateTimingStates(pause);
	
	if (switchNow) {
		// LEDs were on = blink cycle complete -> turn LEDs off, move forward
		if (displayON) {
			endDisplayCycle();
			}
		// LEDs were off -> display at new stroke
		else {
			startDisplayCycle();
		}	
	}	
}
	
void updateLEDVisuals(unsigned int throttleInput) {
	
	updateSystem(assignLEDBlinkPause(throttleInput););
}
