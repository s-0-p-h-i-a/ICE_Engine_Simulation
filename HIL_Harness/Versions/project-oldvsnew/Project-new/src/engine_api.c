/**
Engine 'API':

- Getters to give access to data without exposing managers/init functions
- To be used for data transmission

**/

#include "../lib/strokes.h"
#include "../lib/angles.h"
#include "crank_piston.h"
#include "camshaft.h"
#include "cylinder_head_src/valve_control.h"
#include "engine_manager_getters.h"
#include <stdbool.h>
#include <stdint.h>

// ADD ENGINE DATA: ENGINE ON, ENGINE LOAD, ENGINE 'SPEED'

// ADD THROTTLE AND BRAKE DATA

//// CAMSHAFT MODULE
// STROKE
Strokes camshaft_getStroke(void) {
	 Strokes stroke = getCurrentStroke();
	 return stroke;
}

angle camshaft_getAngle(void) {
	angle camAngle = getCamshaftAngle();
	return camAngle;
}

//// CRANKSHAFT MODULE
// ANGLE
angle crankshaft_getAngle(void) {
	angle crankAngle = getCrankshaftAngle();
	return crankAngle;
}
 
//// VALVES MODULE
// INTAKE
bool valves_getIntakeValveState(void) {
	bool intakeValveState = getIntakeValveState();
	return intakeValveState;
}
// EXHAUST
bool valves_getSxhaustValveState(void) {
	bool exhaustValveState = getExhaustValveState();
	return intakeValveState;
}

//// FUEL AND SPARK MODULE
// FUEL
bool fuelandspark_getFuelInjectorState(void) {
	bool fuelInjectorState = getFuelInjectorState();
	return fuelInjectorState;
}
// SPARK
bool fuelandspark_getSparkPlugState(void) {
	bool sparkPlugState = getSparkPlugState();
	return sparkPlugState;
}
