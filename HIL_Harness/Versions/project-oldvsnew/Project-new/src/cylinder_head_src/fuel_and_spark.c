/**
Fuel injector and spark plug:


**/

#include "../lib/angles.h"
#include "../lib/strokes.h"
#include "./crank_piston_anglegetter.h"
#include "./camshaft_strokegetter.h"
#include "fuel_and_spark.h"
#include <stdint.h>
#include <stdbool.h>

// DEFINITIONS
typedef uint8_t duration_time;
typedef unsigned long timer;

typedef enum {	ALL_OFF = 0,
				FUEL_ON,
				SPARK_ON } FuelAndSparkStates;

typedef enum {	FUELANDSPARK_OK = 0,
				FUELANDSPARK_UPDATE_ERROR } FuelAndSparkErrorStatus;

struct FuelOrSparkElement {
	bool element;
	timer lastTimeSwitchedOn;
	const angle timingAngle;
	const duration_time durationTime;
	const Strokes strokeWhenOn;
};
	
// CONSTANTS
static const bool ON = 1;
static const bool OFF = 0;
static const timer TIMER_ZERO = 0;

static const angle INJECTION_TIMING_OFFSET = 20;
static const angle IGNITION_TIMING_OFFSET = 25;

static const angle INJECTION_TIMING_ANGLE = ANGLE_TDC - INJECTION_TIMING_OFFSET;
static const angle IGNITION_TIMING_ANGLE = ANGLE_TDC - IGNITION_TIMING_OFFSET;

static const uint8_t DURATION_AMPLIFY = 200;
static const duration_time INJECTION_DURATION = 1 * DURATION_AMPLIFY;
static const duration_time IGNITION_DURATION = 2 * DURATION_AMPLIFY;

// VARIABLES
static bool fuelInjector = OFF;
static bool sparkPlug = OFF;

static struct FuelOrSparkElement fuel = { fuelInjector, TIMER_ZERO, INJECTION_TIMING_ANGLE, INJECTION_DURATION, EXHAUST };
static struct FuelOrSparkElement spark = { sparkPlug, TIMER_ZERO, IGNITION_TIMING_ANGLE, IGNITION_DURATION, COMPRESSION };

static angle crankAngleThisLoop = ANGLE_TDC;
static angle crankAngleLastLoop = ANGLE_TDC;
static timer timeNow = TIMER_ZERO;

static FuelAndSparkStates currentState = FUEL_ON; // initialise at intake
static FuelAndSparkErrorStatus errorStatus = FUELANDSPARK_OK;

// FUNCTIONS
// STATIC
static bool checkTimingAnglePassed(angle timingAngle);
static void activate(struct FuelOrSparkElement *element);
static bool checkIfDurationElapsed(struct FuelOrSparkElement *element, struct FuelOrSparkElement *element);

// CHECK IF TRIGGER ANGLE PASSED
static bool checkTimingAnglePassed(angle timingAngle) {
	
	bool passedAngle = (crankAngleLastLoop < timingAngle) && (crankAngleThisLoop >= timingAngle);
	return passedAngle;
}

// ACTIVATE
static void activate(struct FuelOrSparkElement *element) {
	
	element -> element = ON;
	element -> lastTimeSwitchedOn = HAL_GetTick();
}

// CHECK DURATION ELAPSED
static bool checkIfDurationElapsed(struct FuelOrSparkElement *fuelOrSpark) {
	timer timeNow = HAL_GetTick();
	timer delta = timeNow - fuelOrSpark->lastTimeSwitchedOn;
	bool durationPassed = delta > fuelOrSpark->durationTime;
	return durationPassed;
}

static bool updateFuelAndSpark(void) {
	
	switch(currentState) {
		
		case ALL_OFF: {
			Strokes strokeThisLoop = getCurrentStroke();
			
			if (currentStroke == COMPRESSION) {
				if (checkTimingAnglePassed(spark->timingAngle)) {
					activate(&spark);
					currentState = SPARK_ON;
				}
			}
			else if (currentStroke == EXHAUST) {
				if (checkTimingAnglePassed(fuel->timingAngle)) {
					activate(&fuel);
					currentState = FUEL_ON;
				}
			}
		} break;
		
		case FUEL_ON: {
			if (checkIfDurationElapsed(fuel)) {
				fuel->element = OFF;
				currentState = ALL_OFF;
			}
		} break;
		
		case SPARK_ON: {
			if (checkIfDurationElapsed(spark)) {
				spark->element = OFF;
				currentState = ALL_OFF;
			}
		} break;
		
		default: {
			return 0;
		}	
	}
	return 1;
}

// NON STATIC
// INIT
bool fuelAndSparkInit(void) {

	crankAngleThisLoop = ANGLE_TDC;
	crankAngleLastLoop = ANGLE_TDC;
	timeNow = TIMER_ZERO;
	currentState = FUEL_ON;
	errorStatus = FUELANDSPARK_OK;
	return 1;
}

// UPDATE
bool manageFuelAndSparkLines(void) {
	
	if (!updateFuelAndSpark()) {
		errorStatus = FUELANDSPARK_UPDATE_ERROR;
		return 0;
	}
	errorStatus = FUELANDSPARK_OK;
	return 1;
}

// GETTERS
// FUEL
bool getFuelInjectorState(void) {
	return fuel->element;
}
//SPARK
bool getSparkPlugState(void) {
	return spark->element;
}
