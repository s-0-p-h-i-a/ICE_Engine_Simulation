#include "../lib/strokes.h"
#include "./engine_api_fuel_spark.h"
#include "fuel_and_spark_lines.h"
#include <stdint.h>
#include <stdbool.h>

// CONSTANTS
static const bool ON = 1;
static const bool OFF = 0;

static const fuelInjectorStates[TOTAL_STROKES] = { ON, OFF, OFF, OFF };
static const sparkPlugStates[TOTAL_STROKES] = { OFF, OFF, ON, OFF };

// VARIABLES
static bool fuelInjector = ON;
static bool sparkPlug = OFF;

// FUNCTIONS
bool updateFuelAndSparkLines(void) {
	
	Strokes newIndex = getCurrentStrokeIndex();
	
	fuelInjector = fuelInjectorStates[newIndex];
	sparkPlug = sparkPlugStates[newIndex];
	
	return 1;
}

bool getFuelInjectorState(void) {
	return fuelInjector;
}

bool getSparkPlugState(void) {
	return sparkPlug;
}
