#include "cylinder.h"
#include <stdint.h>

//// CONSTANTS
#define intakeValveStates 	{ 1, 0, 0, 0 }
#define exhaustValveStates 	{ 0, 0, 0, 1 }
#define fuelInjectorStates 	{ 1, 0, 0, 0 }
#define sparkPlugStates 	{ 0, 0, 1, 0 }
#define pistonStates 		{ 0, 1, 0, 1 }

typedef enum { INTAKE_VALVE = 0, EXHAUST_VALVE = 1, FUEL_INJECTOR = 2, SPARK_PLUG = 3, PISTON = 4, TOTAL = 5 } Elements;
typedef enum { INTAKE = 0, COMPRESSION = 1, COMBUSTION = 2, EXHAUST = 3 } Strokes;
typedef enum { TOTAL_CYCLE = 4, CYLINDER_ELEMENTS = 5 } Sizes;

static const uint8_t cylinderStatesMatrix[CYLINDER_ELEMENTS][TOTAL_CYCLE] = 
	{ intakeValveStates, exhaustValveStates, fuelInjectorStates, sparkPlugStates, pistonStates };

//// FLAG & STATES
static Strokes currentCylinderState = INTAKE;

int cylinderStateNow = INTAKE;

uint8_t cylinderElementStates[CYLINDER_ELEMENTS] = { 1, 0, 1, 0, 0 }; // initialise at intake


//// FUNCTIONS 
void updateCylinderElements(int state) {
	
	for (int i = 0; i < TOTAL; ++i) {
		cylinderElementStates[i] = cylinderStatesMatrix[i][state]; 
	}
}

void updateCylinderState(void) {
	
	if (currentCylinderState == EXHAUST) { currentCylinderState = INTAKE; }
	else { ++currentCylinderState; }
	
	cylinderStateNow = currentCylinderState;
	
	updateCylinderElements(currentCylinderState);
}
