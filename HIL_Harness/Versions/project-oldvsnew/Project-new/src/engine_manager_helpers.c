/**
Engine manager helpers:

- Initialisation and reset
- Manager process helpers
- Clamp/compute helpers
- Getters for data logging

**/

#include "engine_manager_helpers.h"
#include "engine_manager_getters.h"
#include "throttle_brake_ignition.h"
#include <stdint.h>
#include <stdbool.h>

// DEFINITIONS
typedef enum {	ENGINE_SYSTEM_OK,
				INIT_ENGINE_ERROR,
				UPDATE_ENGINE_ERROR,
				ENGINE_TOGGLE_ERROR,
				MANAGE_ENGINE_STATE_ERROR } EngineSystemStatuses;
							
typedef enum {	ENGINE_IDLE,
				ENGINE_TOGGLE,
				ENGINE_INPUT,
				ENGINE_COAST,
				ENGINE_AUTO_DECEL,
				ENGINE_OFF,
				ENGINE_UNDEFINED } EngineStates;
				
typedef uint8_t engine_load;
typedef int8_t engine_input;

// CONSTANTS
const engine_load ZERO_LOAD	= 0;
const engine_load IDLE_LOAD	= 20;
const engine_load MAX_LOAD	= 100;
const engine_input MAX_THROTTLE_INPUT	= 10;
const engine_input MAX_BRAKE_INPUT		= -10;
const uint8_t DECEL_RATE = 1;

// FUNCTIONS
// RESET
bool resetToIdle(void) {
	
	engineState = ENGINE_IDLE;
	engineLoad = IDLE_LOAD;
	engineSystemStatus = ENGINE_SYSTEM_OK
	engineON = 1;
	
	return 1;
}

// CHECK TOGGLE
void handleToggleInput(void) {
		
	engineON = !engineON;
		
	if (engineON) {
		resetToIdle();
	} else {
		engineSystemInit();
	}
}

// GET INPUT
engine_input getNewInput(void) {
	
	engine_input newThrottleInput = readThrottle();
	engine_input newBrakeInput = readBrake();
	
	if (newBrakeInput < MAX_BRAKE_INPUT) {
		newBrakeInput = MAX_BRAKE_INPUT;
	}
	
	if (newThrottleInput > MAX_THROTTLE_INPUT) {
		newThrottleInput = MAX_THROTTLE_INPUT;
	}
	
	engine_input newInput = newThrottleInput + newBrakeInput;
	
	return newInput;
}

// CLAMP LOAD VALUE
engine_load clampEngineLoad(int8_t newLoad) {
	
	if (newLoad > MAX_LOAD) {
		newLoad = MAX_LOAD;
	}
	
	if (newLoad < IDLE_LOAD) {
		newLoad = IDLE_LOAD;
	}
	return newLoad;
}

// COMPUTE NEW LOAD
engine_load computeNewLoad(void) {
	
	if ((engineLoad > IDLE_LOAD) && (engineLoad < MAX_LOAD)) {
		engineLoad = clampEngineLoad(engineLoad + getNewInput());
	}
	return engineLoad;
}

// FINISH ROUTINE
void finishEngineRoutine(EngineSystemStatuses newStatus) {

	if ((newStatus != SYSTEM_OK) && (engineON)) {
		resetToIdle();
	}
	
	engineSystemStatus = newStatus;
}

// GETTERS
engine_load getEngineLoad(void) {
	return engineLoad;
}

EngineStates getEngineState(void) {
	return EngineState;
}

EngineSystemStatuses getEngineSystemStatus(void) {
	return EngineSystemStatus;
}
