#include "camshaft.h"
#include "valve_control.h"
#include <stdint.h>
#include <stdbool.h>

// CONSTANTS
typedef enum {	CAM_OK,
				CAM_UPDATE_ERROR } CamSystemStatuses;

static const uint8_t ZERO = 0;
static const uint8_t TWO = 2;
static const uint8_t GEAR_RATIO = 2;

// VARIABLES
static angle_position camshaftAngle = ANGLE_ZERO;
static CamSystemStatuses camshaftSystemStatus = CAM_OK;

// FUNCTIONS
bool updateCamshaft(angle_position crankAngle) {
	
	camshaftSystemStatus = CAM_OK;
	
	if (crankAngle % TWO != ZERO) {
		return 1;
	}
		
	if (crankAngle == ANGLE_ZERO) {
		if (!updateValves()) {
			camshaftSystemStatus = CAM_UPDATE_ERROR;
			return 0;
		}
	}
	
	camshaftAngle = crankAngle / GEAR_RATIO;
	
	return 1;
}

bool updateCamshaft(angle_position crankAngle) {
	
	camshaftSystemStatus = CAM_OK;
	
	if (crankAngle % TWO != ZERO) {
		return 1;
	}
	
	camshaftAngle = crankAngle / GEAR_RATIO;
		
	if (camshaftAngle == ANGLE_ZERO) {
		if (!updateValves()) {
			camshaftSystemStatus = CAM_UPDATE_ERROR;
			return 0;
		}
	}
	
	return 1;
}
