#include "global.h"
#include "engine.h"
#include <stdbool.h>

static const int strokes[TOTAL_CYCLE] 				= { INTAKE, COMPRESSION, COMBUSTION, EXHAUST};

static const bool intakeValveCycle[TOTAL_CYCLE] 	= { true, false, false, false };
static const bool exhaustValveCycle[TOTAL_CYCLE]	= { false, false, false, true };
static const bool fuelInjectionCycle[TOTAL_CYCLE]	= { true, false, false, false };
static const bool sparkPlugCycle[TOTAL_CYCLE]		= { false, false, true, false };
static const bool pistonCycle[TOTAL_CYCLE]			= { BDC, TDC, BDC, TDC };

/*static const bool intakeStates[CYLINDER_ELEMENTS] 		= { true, 	false,	 true, false, BDC};
static const bool compressionStates[CYLINDER_ELEMENTS] 	= { false, 	false, 	false, false, TDC};
static const bool combustionStates[CYLINDER_ELEMENTS] 	= { false, 	false, 	false, true,  BDC};
static const bool exhaustStates[CYLINDER_ELEMENTS] 		= { false, 	true, 	false, false, TDC};*/

static const bool statesMap[CYLINDER_ELEMENTS][TOTAL_CYCLE] =
	{ intakeValveCycle, exhaustValveCycle, fuelInjectionCycle, sparkPlugCycle, pistonCycle };

bool cylinder[CYLINDER_ELEMENTS];

int currentStroke = INTAKE;

bool isIdle = true;
bool engineON = false;
void updateStroke(void) {
	
	if (currentStroke == EXHAUST) { currentStroke = INTAKE;}
	
	for (int i = INTAKE; i < EXHAUST; ++i) {
		
		if (i == currentStroke) {
			currentStroke = strokes[i+1];
		}
	}
}
	
	/*if (currentStroke == INTAKE) { currentStroke = COMPRESSION;}
	else if (currentStroke == COMPRESSION) { currentStroke = COMBUSTION;}
	else if (currentStroke == COMBUSTION) { currentStroke = EXHAUST;}
	else if (currentStroke == EXHAUST) { currentStroke = INTAKE;}*/

void startEngine(void) {
	
	for (int i = ZERO; i < CYLINDER_ELEMENTS; ++i) {
		
		cylinder[i] = statesMap[i][INTAKE];
	}
}

void runEngine(void) {
	
	updateStroke();
	
	for (int i = ZERO; i < CYLINDER_ELEMENTS; ++i) {
		
		cylinder[i] = statesMap[i][currentStroke];
	}
}
