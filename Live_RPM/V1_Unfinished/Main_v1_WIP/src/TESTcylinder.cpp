/*
Cylinder System:

Function / Role:
- Internal cylinder object 1x5 array representing cylinder elements and their current states (on/off, TDC/BDC):
	- Intake valve
	- Exhaust valve
	- Fuel injector
	- Spark plug
	- Piston
- Used by the displayCylinder module to control blinking LED display
- Updated by displayCylinder at every LED blink cycle

Process:
- Cylinder element states across the 4-stroke cycle stored in reference matrix
- Internal stroke/state variable updated at every LED blink cycle
- Cylinder object internally updated using current stroke as index to read reference matrix

Driven by:
displayCylinder in cylinderVisuals.cpp via updateCylinderState call

*/

#include "cylinder.h"
#include <stdint.h>

//	========= CONSTANTS =========
// Cylinder element states across the 4 strokes
#define intakeValveStates 	{ 1, 0, 0, 0 }
#define exhaustValveStates 	{ 0, 0, 0, 1 }
#define fuelInjectorStates 	{ 1, 0, 0, 0 }
#define sparkPlugStates 	{ 0, 0, 1, 0 }
#define pistonStates 		{ 0, 1, 0, 1 }

// Cylinder elements
typedef enum { INTAKE_VALVE = 0, EXHAUST_VALVE = 1, FUEL_INJECTOR = 2, SPARK_PLUG = 3, PISTON = 4} Elements;// 4-Stroke // 4-Stroke cycle
typedef enum { INTAKE = 0, COMPRESSION = 1, COMBUSTION = 2, EXHAUST = 3 } Strokes;
// Array sizes
typedef enum { TOTAL_CYCLE = 4, CYLINDER_ELEMENTS = 5 } Sizes;

// Cylinder elements states matrix
static const uint8_t cylinderStatesMatrix[CYLINDER_ELEMENTS][TOTAL_CYCLE] = 
	{ intakeValveStates, exhaustValveStates, fuelInjectorStates, sparkPlugStates, pistonStates };

// 	========= VARIABLES =========
// Current stroke state
static Strokes currentCylinderState = INTAKE;

// Cylinder elements array
static uint8_t cylinderElementStates[CYLINDER_ELEMENTS] = { 1, 0, 1, 0, 0 }; // initialise at intake

// 	========= FUNCTIONS ========= 
// Helper functions
static void updateStroke(void);
static void updateCylinderElements(void);

// Accessor for plotter display
Strokes getCurrentCylinderState(void) { return currentCylinderState; }

// Move forward in 4-stroke cycle
static void updateStroke(void) {
	if (currentCylinderState == EXHAUST) { currentCylinderState = INTAKE; }
	else 								 { ++currentCylinderState; }
}

// Update individual cylinder element states
static void updateCylinderElements(void) {
	
	for (int i = 0; i < CYLINDER_ELEMENTS; ++i) {
		cylinderElementStates[i] = cylinderStatesMatrix[i][currentCylinderState]; 
	}
}

// Full cylinder system update
void updateCylinderState(void) {
	
	updateStroke();
	updateCylinderElements();
}
