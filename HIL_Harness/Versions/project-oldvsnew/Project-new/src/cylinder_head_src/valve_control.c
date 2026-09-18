#include "../lib/strokes.h"
#include "valve_control.h"
#include <stdint.h>
#include <stdbool.h>

// CONSTANTS
static const bool OPEN = 1;
static const bool CLOSED = 0;

// VARIABLES

// system initialises at intake stroke, both valves are open at the start
static bool intakeValveState = OPEN;
static bool exhaustValveState = OPEN;

// FUNCTIONS
// INIT
bool valvesInit(void) {
	intakeValveState = OPEN;
	exhaustValveState = OPEN;
	return 1;
}
// UPDATE
// called when cam angle reaches trigger edge
bool updateValves(Strokes currentStroke) {
	
	switch (currentStroke) {
		
		case INTAKE: {
			exhaustValveState = CLOSED;
		} break;
			
		case COMPRESSION: {
			intakeValveState = CLOSED;
		} break;
			
		case COMBUSTION: {
			exhaustValveState = OPEN;
		} break;
			
		case EXHAUST: {
			intakeValveState = OPEN;
		} break;
		
		default: {
			return 0;
		}
	}
	return 1;
}

bool getIntakeValveState(void) {
	return currentValveStates[INTAKE_VALVE_ELEMENT];
}

bool getExhaustValveState(void) {
	return currentValveStates[EXHAUST_VALVE_ELEMENT];
}
