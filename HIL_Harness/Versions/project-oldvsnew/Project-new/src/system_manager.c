/**
System manager:

Central execution manager:

	- Calls engine manager + event handling + data transmission functions
	- Tracks system initialisation and system manager execution statuses

**/

#include "system_manager_main.h"
#include "system_manager_getters.h"
#include "event_handler.h"
#include "engine_manager.h"
#include <stdbool.h>
#include <stdint.h>

// CONSTANTS / DEFINITIONS

// => TO BE PUT IN GETTERS HEADER
typedef enum {	INIT_OK = 0,
				HARDWARE_INIT_ERROR,
				ENGINE_INIT_ERROR,
				HW_AND_ENGINE_INIT_ERROR } InitStatuses;
				
typedef enum {	SYSTEM_OK = 0,
				INIT_ERROR,
				ENGINE_MANAGER_ERROR,
				SEND_DATA_ERROR } SystemStatuses;
// => TO BE PUT IN GETTERS HEADER

// VARIABLES
static InitStatuses initStatus = INIT_OK;
static bool hardwareInitOK = 1;
static bool engineInitOK = 1;
static SystemStatuses systemStatus = SYSTEM_OK;

// FUNCTIONS
// INIT
void systemInit(void) {
	
	initStatus = INIT_OK;
	hardwareInitOK = 1;
	engineInitOK = 1;
	systemStatus = SYSTEM_OK;
	
	if (!hardwareInit()) {
		hardwareInitOK = 0;
		initStatus = HARDWARE_INIT_ERROR;
	}
	
	if (!engineSystemInit()) {
		engineInitOK = 0;
		initStatus = hardwareInitOK ? ENGINE_INIT_ERROR : HW_AND_ENGINE_INIT_ERROR;
	}
	
	if (initStatus != INIT_OK) {
		systemStatus = INIT_ERROR;
	}
}

// NON-STATIC
// MANAGER
bool runSystemManager(void) {
	
	if (systemStatus == INIT_ERROR) {
		return 0;
	}

	bool newInputThisLoop = eventDetected();

	if (!runEngineManager(newInputThisLoop)) {
		systemStatus = ENGINE_MANAGER_ERROR;
	}
	
	if (!sendNewDataLog()) {
		systemStatus = SEND_DATA_ERROR;
	}
	
	bool returnValue = systemStatus == SYSTEM_OK;
	return returnValue;
}

// GETTERS
// SYSMANAGER STATUS
SystemStatuses getSystemManagerStatus(void) {
	return systemStatus;
}

// INIT STATUS
InitStatuses getInitialisationStatus(void) {
	return initStatus;
}
