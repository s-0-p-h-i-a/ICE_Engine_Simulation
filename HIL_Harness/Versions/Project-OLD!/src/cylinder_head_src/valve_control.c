#include "../lib/strokes.h"
#include "valve_control.h"
#include <stdint.h>
#include <stdbool.h>

// CONSTANTS
static const bool OPEN = 1;
static const bool CLOSED = 0;

static const size_t TOTAL_VALVES = 2;

static const uint8_t INTAKE_VALVE_ELEMENT = 0;
static const uint8_t EXHAUST_VALVE_ELEMENT = 1;

static const intakeValveStates[TOTAL_STROKES] = { OPEN, CLOSED, CLOSED, CLOSED };
static const exhaustValveStates[TOTAL_STROKES] = { CLOSED, CLOSED, CLOSED, OPEN };

// VARIABLES
static Strokes currentIndex = INTAKE;

static bool currentValveStates[TOTAL_VALVES] = { OPEN, CLOSED };

// FUNCTIONS
bool updateValves(void) {
	
	currentIndex = advanceStroke(currentIndex);
	
	currentValveStates[INTAKE_VALVE_ELEMENT] = intakeValveStates[currentIndex];
	currentValveStates[EXHAUST_VALVE_ELEMENT] = exhaustValveStates[currentIndex];
	
	return 1;
}

Strokes getCurrentIndex(void) {
	return currentIndex;
}

bool getIntakeValveState(void) {
	return currentValveStates[INTAKE_VALVE_ELEMENT];
}

bool getExhaustValveState(void) {
	return currentValveStates[EXHAUST_VALVE_ELEMENT];
}
