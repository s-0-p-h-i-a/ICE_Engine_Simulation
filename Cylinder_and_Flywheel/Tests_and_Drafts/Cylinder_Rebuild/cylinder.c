#include "cylinder.h"
#include <stdbool.h>

#define INTAKE      0
#define COMPRESSION 1
#define COMBUSTION  2
#define EXHAUST     3

#define TOTAL_CYCLE 4
#define CYLINDER_ELEMENTS 5

static const int INTAKE_VALVE    = 0;
static const int EXHAUST_VALVE   = 1;
static const int FUEL_INJECTOR   = 2;
static const int SPARK_PLUG      = 3;
static const int PISTON          = 4;

static const bool intakeValveStates[TOTAL_CYCLE] = { true, false, false, false };
static const bool exhaustValveStates[TOTAL_CYCLE] = { false, false, false, true };
static const bool fuelInjectorStates[TOTAL_CYCLE] = { true, false, false, false };
static const bool sparkPlugStates[TOTAL_CYCLE] = { false, false, true, false };
static const bool pistonStates[TOTAL_CYCLE] = { false, true, false, true };

static const bool cylinderMatrix[CYLINDER_ELEMENTS][TOTAL_CYCLE] = 
	{ {intakeValveStates}, {exhaustValveStates}, {fuelInjectorStates}, {sparkPlugStates}, {pistonStates} };

bool switchCylinderState = false;
static int currentCylinderState = INTAKE;
bool cylinderElementStates[CYLINDER_ELEMENTS] = { true, false, true, false, false }; // initialise at intake

void updateCylinderState(void) {
	
	if (currentCylinderState == EXHAUST) { currentCylinderState = INTAKE; }
	else { ++currentCylinderState; }
}

void updateCylinderElements(void) {
	
	for (int i = INTAKE_VALVE; i <= PISTON; ++i) {
		cylinderElementStates[i] = cylinderMatrix[i][currentCylinderState];
	}
}

void updateCylinder(void) {
	updateCylinderState();
	updateCylinderElements();
}

void runCylinder(void) {
	if (switchCylinderState) { updateCylinder(); }
}
