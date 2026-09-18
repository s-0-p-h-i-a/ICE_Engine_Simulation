#include "cylinder.h"
#include "cylinderVisuals.h"
#include "crankshaftVisuals.h"

//	========= CONSTANTS =========
#define CRANKSHAFT 0
#define LEDS 1
#define TOTAL_ELEMENTS 2
#define TOTAL_STATES 4
#define PAUSE 100
#define PHASE_STEP 1000

// 4-Stroke Cycle
typedef enum { INTAKE = 0, COMPRESSION = 1, COMBUSTION = 2, EXHAUST = 3 } Strokes;
// Cylinder LED display timing levels
typedef enum { IDLE_INCREMENT = 10, SPEED_ONE_INCREMENT = 100, SPEED_TWO_INCREMENT = 500 } PhaseIncrement;

// CONTROL ARRAYS:
// Blink pauses array
static const unsigned int phaseIncrementLevels[TOTAL_STATES] = { IDLE_INCREMENT, SPEED_ONE_INCREMENT, SPEED_TWO_INCREMENT };

// 	========= VARIABLES =========
// Current stroke
static unsigned int currentStroke = INTAKE;

static bool displayON = 0;
static unsigned int currentPhase = 0;

// 	========= FUNCTIONS ========= 
// Helper functions
static unsigned int assignLEDBlinkPause(unsigned int throttleInput);
static void updateStroke(void);
static void updateTimingStates(unsigned int pause);
static void updateDisplay(void);

// Accessor for plotter display
Strokes getCurrentStroke(void) { return currentStroke; }

// Set cylinder LED display blink pause
static unsigned int assignPhaseIncrement(unsigned int throttleInput) {

	return phaseIncrementLevels[throttleInput];
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
static bool updatePhase(unsigned int increment) {

	// Timestamp and delta
	unsigned long currentTime = millis();
	unsigned long delta = currentTime - lastSwitch;

	// Update control flags and timestamp
	if (delta >= PAUSE) {
		
		currentPhase += increment;
		
		if (currentPhase >= PHASE_STEP) {
			currentPhase -= PHASE_STEP;
			return 1;
		} else {
			return 0;
		}
		lastSwitch += PAUSE;
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

static void updateSystem(unsigned int increment) {
	
	bool switchNow = updatePhase(increment);
	
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
	
void advanceCrankshaft(unsigned int throttleInput) {
	
	updateSystem(phaseIncrementLevels[throttleInput]);
}
