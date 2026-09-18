#include "error_handling_JOYSTICK.h"
#include "control_RED.h"
#include "control_GREEN.h"
#include "control_BLUE.h"
#include "range_handling.h"
#include "error_reports.h"
#include <stddef.h>
#include <stdio.h>

#define ARRAY_BUILD_STATUS_MESSAGE "ARRAY_BUILD_ERROR"

typedef enum {	ALL_MODULES_OK = 0,
				ONE_MODULE_HAS_ERROR,
				TWO_MODULES_HAVE_ERROR,
				THREE_MODULES_HAVE_ERROR,
				UNDEFINED_ERROR } SystemErrors;

static const size_t SYSTEM_REPORTS_ARRAY_SIZE = 5;
				
SystemErrors *errorReportArray[SYSTEM_REPORTS_ARRAY_SIZE] = { NULL };

ReportArrayBuildProcessPhases reportArrayBuildStatus = NO_ARRAY_BUILD_ERRORS;

static bool assignJoystickStatusElements(void);
static bool assignColourStatusElements(void);
static bool assignSystemReportCode(void);
static SystemErrors computeSystemReportCode(void);

static bool assignJoystickStatusElements(void) {
	
	for (int i = 0; i < JOYSTICK_ERROR_TRACKING_ARRAY_SIZE; ++i) {
		*errorReportArray[i] = getJoystickSystemStateArray(i);
		if (errorReportArray[i] == NULL) {
			return 0;
		}
	}
	return 1;
}

static bool assignColourStatusElements(void) {
	
	for (int i = RED_POSITION; i <= BLUE_POSITION; ++i) {
		uint8_t colourStatus = NO_RANGEHANDLING_ERRORS;
		
		switch(i) {
			case RED_POSITION: {
				colourStatus = getRedRangeHandlingStatus();
			} break;
			case GREEN_POSITION: {
				colourStatus = getGreenRangeHandlingStatus();
			} break;
			case BLUE_POSITION: {
				colourStatus = getBlueRangeHandlingStatus();
			}
			default: {
				return 0;
			}
		}
		*errorReportArray[i] = colourStatus;
		
		if (errorReportArray[i] == NULL) {
			return 0;
		}
	}
	return 1;
}

static SystemErrors computeSystemReportCode(void) {
	
	bool redError = *errorReportArray[RED_POSITION] > 0;
	bool greenError = *errorReportArray[GREEN_POSITION] > 0;
	bool blueError = *errorReportArray[BLUE_POSITION] > 0;
	
	bool allModulesOK = !(redError || greenError || blueError);
	
	uint8_t totalErrors = *errorReportArray[RED_POSITION] + *errorReportArray[GREEN_POSITION] + *errorReportArray[BLUE_POSITION];
	
	bool oneModuleError = totalErrors == ONE_MODULE_HAS_ERROR;
	bool twoModulesError = totalErrors == TWO_MODULES_HAVE_ERROR;
	bool threeModulesError = totalErrors == THREE_MODULES_HAVE_ERROR;
	
	if (allModulesOK) {
		return ALL_MODULES_OK;
	} else if (oneModuleError) {
		return ONE_MODULE_HAS_ERROR;
	} else if (twoModulesError) {
		return TWO_MODULES_HAVE_ERROR;
	} else if (threeModulesError) {
		return THREE_MODULES_HAVE_ERROR;
	}
}

static bool assignSystemReportCode(void) {
	
	*errorReportArray[SYSTEM_STATE_POSITION] = computeSystemReportCode();
	
	if (errorReportArray[SYSTEM_STATE_POSITION] == NULL) {
			return 0;
		}
	return 1;
}

void printErrorReportArrayBuildError(void) {
	printf(" %c%d /n", ARRAY_BUILD_STATUS_MESSAGE, reportArrayBuildStatus);
}

extern uint8_t getErrorReportArrayElements(uint8_t index) {
	return *errorReportArray[index];
}

bool generateErrorReportArray(void) {
	
	bool buildStatus = assignJoystickStatusElements();
	
	if (!buildStatus) {
		reportArrayBuildStatus = JOYSTICK_STATUS_ASSIGNMENT;
		return 0;
	}
	
	buildStatus = assignColourStatusElements();
	
	if (!buildStatus) {
		reportArrayBuildStatus = COLOUR_STATUS_ASSIGNMENT;
		return 0;
	}
	
	buildStatus = assignSystemReportCode();

	if (!buildStatus) {
		reportArrayBuildStatus = SYSTEM_STATUS_ASSIGNMENT;
		return 0;
	}
	return 1;
}
