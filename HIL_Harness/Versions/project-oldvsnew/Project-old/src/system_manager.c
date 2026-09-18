#include "system_manager.h"
#include "engine_manager.h"
#include <stdbool.h>

// CONSTANTS
typedef enum {	SYSTEM_OK = 0,
				UPDATE_SYSTEM_STATE_ERROR,
				MANAGE_STATE_ERROR,
				PREPARE_NEXT_LOOP_ERROR } SystemStatuses;

typedef enum {	NO_INTERRUPT = 0,
				INTERRUPT_DETECTED } SystemManagerStates;

// VARIABLES
SystemManagerStates systemManagerState = NO_INTERRUPT;

// FUNCTIONS
bool runSystemManager(void) {
	
	systemManagerState = NO_INTERRUPT;
	
	if (interruptDetected()) {
		resetInterruptTimer();
		systemManagerState = INTERRUPT_DETECTED;
	}
	
	runEngineManager();
	return 1;
}

bool getSystemManagerState(void) {
	return systemManagerState;
}
