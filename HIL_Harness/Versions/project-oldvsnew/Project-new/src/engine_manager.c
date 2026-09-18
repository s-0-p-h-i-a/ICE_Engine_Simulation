/**
Engine manager:

- Central manager of simulated engine
- Calls crank and fuel+spark lines updates
- Reads input from ignition (on/off toggle), and throttle and brake (calculate new engine load)

**/

#include "engine_manager.h"
#include "engine_manager_helpers.h"
#include "cylinder_head_src/fuel_and_spark.h"
#include <stdint.h>
#include <stdbool.h>
		
// VARIABLES

// DECLARE IN HELPERS HEADER
bool engineON = 0;
bool newInputThisLoop = 0;
engine_load engineLoad = ZERO_LOAD;

EngineStates engineState = ENGINE_UNDEFINED;
EngineSystemStatuses engineSystemStatus = ENGINE_SYSTEM_OK;
// DECLARE IN HELPERS HEADER

// FUNCTIONS
// STATIC
static bool updateEngineState(void);
static bool manageCurrentEngineState(void);

// UPDATE STATE
static bool updateEngineState(void) {
	
	switch (engineState) {
		
		case ENGINE_IDLE: {
			if (newInputThisLoop) {
				engineState = ENGINE_INPUT;
			}
		} break;
		
		case ENGINE_INPUT: {
			if (!newInputThisLoop) {
				engineState = ENGINE_COAST;
			}
		} break;
		
		case ENGINE_COAST: {
			if (coastTimerElapsed()) {
				engineState = ENGINE_AUTO_DECEL;
			}
		} break;
		
		case ENGINE_AUTO_DECEL: {
			if (engineLoad == IDLE_LOAD) {
				engineState = ENGINE_IDLE;
			}
		} break;
		
		case ENGINE_OFF:
			break;
		
		default: {
			return 0;
		}
	}
	return 1;
}

// MANAGE STATE
static bool manageCurrentEngineState(void) {
	
	switch (engineState) {
		
		case ENGINE_IDLE: {
			engineLoad = IDLE_LOAD;
		} break;
		
		case ENGINE_INPUT: {
			engineLoad = computeNewLoad();
		} break;
		
		case ENGINE_COAST:
			break;
		
		case ENGINE_AUTO_DECEL: {
			engineLoad = clampEngineLoad(engineLoad - DECEL_RATE);
		} break;
		
		case ENGINE_OFF: {
			return 1;
		}
			
		default:
			return 0;
	}

	if (!updateCrank(engineLoad)) {
		return 0;
	}
	
	if (!manageFuelAndSparkLines()) {
		return 0;
	}
	
	return 1;	
}

// NON-STATIC
// INIT
// INIT
bool engineSystemInit(void) {
	
	engineState = ENGINE_OFF;
	engineSystemStatus = ENGINE_SYSTEM_OK;
	engineON = 0;
	engineLoad = ZERO_LOAD;
	
	if (!crankshaftInit()) {
		return 0;
	}
	if (!fuelAndSparkInit()) {
		return 0;
	}
	return 1;
}
// MANAGER
bool runEngineManager(bool newInputNow) {
	
	newInputThisLoop = newInputNow;
	
	if (newEngineToggle()) {
		handleToggleInput();
	}
	
	if (!updateEngineState()) {
		finishEngineRoutine(UPDATE_ENGINE_ERROR);
		return 0;
	}	
	
	if (!manageCurrentEngineState()) {
		finishEngineRoutine(MANAGE_ENGINE_STATE_ERROR);
		return 0;
	}
	
	finishEngineRoutine(ENGINE_SYSTEM_OK);
	return 1;
}
