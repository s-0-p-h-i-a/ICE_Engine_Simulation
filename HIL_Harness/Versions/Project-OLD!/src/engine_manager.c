#include "engine_manager.h"
#include "throttle_and_brake.h"
#include "fuel_and_spark_lines.h"
#include <stdint.h>
#include <stdbool.h>

//// CONSTANTS / DEFINES

typedef enum {	ENGINE_SYSTEM_OK,
				UPDATE_ENGINE_ERROR,
				MANAGE_ENGINE_STATE_ERROR } EngineSystemStatuses;
							
typedef enum {	ENGINE_IDLE,
				ENGINE_INPUT,
				ENGINE_COAST,
				ENGINE_AUTO_DECEL,
				ENGINE_UNDEFINED } EngineStates;
				
typedef uint8_t engine_load;

static const engine_load ZERO_LOAD	= 0;
static const engine_load IDLE_LOAD	= 20;
static const engine_load MAX_LOAD	= 100;
static const uint8_t MAX_THROTTLE_INPUT	= 100;
static const int8_t MAX_BRAKE_INPUT		= -100;
static const uint8_t DECEL_RATE = 10;
		
//// VARIABLES
bool engineON = 0;
bool newInputThisLoop = 0;
bool changeStateNextLoop = 0;
engine_load engineLoad = ZERO_LOAD;

EngineStates engineState = ENGINE_UNDEFINED;
EngineSystemStatuses engineSystemStatus = ENGINE_SYSTEM_OK;

//// FUNCTIONS
static bool resetToIdle(void);
static bool updateEngineState(void);
static bool manageCurrentEngineState(void);
static bool finishEngineRoutine(EngineSystemStatuses newStatus);

//// STATIC

// RESET
static bool resetToIdle(void) {
	engineState = ENGINE_IDLE;
	engineLoad = IDLE_LOAD;
	return 1;
}

// UPDATE STATE
static bool updateEngineState(void) {
	
	switch (engineState) {
		
		case ENGINE_IDLE: {
			if (newInputThisLoop) {
				engineState = ENGINE_INPUT;
				return 1;
			} else {
				return 1;
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
		
		default: {
			engineState = ENGINE_COAST;
		}
	}
}

// GET HW INPUT
static int8_t getNewInput(void) {

	int8_t newInput = readThrottle() + readBrake();
	
	if (newInput < MAX_BRAKE_INPUT) {
		newInput = MAX_BRAKE_INPUT;
	}
	
	if (newInput > MAX_THROTTLE_INPUT) {
		newInput = MAX_THROTTLE_INPUT;
	}
	return newInput;
}

// CLAMP LOAD VALUE
static engine_load clampEngineLoad(int8_t newLoad) {
	
	if (newLoad > MAX_LOAD) {
		newLoad = MAX_LOAD;
	}
	
	if (newLoad < IDLE_LOAD) {
		newLoad = IDLE_LOAD;
	}
	return newLoad;
}

// MANAGE STATE
static bool manageCurrentEngineState(void) {
	
	switch (engineState) {
		
		case ENGINE_IDLE: {
			engineLoad = IDLE_LOAD;
		} break;
		
		case ENGINE_INPUT: {
			engineLoad += clampEngineLoad(getNewInput());
		} break;
		
		case ENGINE_COAST:
			break;
		
		case ENGINE_AUTO_DECEL: {
			engineLoad -= clampEngineLoad(DECEL_RATE);
		} break;
		
		case ENGINE_UNDEFINED:
		default:
			return 0;
			break;
	}

	if (!updateCrank(engineLoad)) {
		return 0;
	}
	
	if (!updateFuelAndSparkLines()) {
		return 0;
	}
	
	return 1;	
}

// FINISH ROUTINE
static bool finishEngineRoutine(EngineSystemStatuses newStatus) {
	
	if (!engineON()) {
		engineSystemInit();
	}
	
	if (newStatus != SYSTEM_OK) {
		resetToIdle();
	}
	
	engineSystemStatus = newStatus;
	return 1;
}

// INIT
bool engineSystemInit(void) {
	
	engineState = ENGINE_UNDEFINED;
	
	if (!crankshaftInit()) {
		return 0;
	}
	return 1;
}

//// MANAGER
bool runEngineManager(void) {
	
	if (!updateEngineState()) {
		finishEngineRoutine(UPDATE_ENGINE_ERROR);
		return 0;
	}
	
	if (!engineON) {
		finishEngineRoutine(ENGINE_SYSTEM_OK);
		return 1;
	}
	
	newInputThisLoop = getSystemManagerState();
	
	if (!manageCurrentEngineState()) {
		finishEngineRoutine(MANAGE_ENGINE_STATE_ERROR);
		return 0;
	}
	
	finishEngineRoutine(ENGINE_SYSTEM_OK);
	return 1;
}
