/**
Event handler:

- Polls system every ~50ms, returns whether new hardware input was detected
- Lower listen frequency intentionally chosen to make system less responsive for better data visualisation on monitor

**/

#include "event_handler.h"
#include "event_handler_getter.h"
#include <stdbool.h>

// TYPEDEF
typedef unsigned long eventHandlerTime;

static const bool NO_EVENT_THIS_LOOP = 0;
static const bool EVENT_THIS_LOOP = 1;

// CONSTANTS
static const eventHandlerTime TIMER_ZERO = 0;
static const eventHandlerTime TIMER_PHASE = 100;

// VARIABLES
static eventHandlerTime lastPollTime = TIMER_ZERO;
static bool eventHandlerStateThisLoop = NO_EVENT_THIS_LOOP;

// FUNCTIONS
// STATIC
static bool listenNow(void);

// LISTEN FOR INPUT
static bool listenNow(void) {
	
	eventHandlerTime timeNow = HAL_GetTick();
	unsigned long delta = timeNow - lastPollTime;
	
	bool phaseElapsed = delta >= TIMER_PHASE;
	
	if (phaseElapsed) {
		lastPollTime = timeNow;
	}
	
	return phaseElapsed;
}

// NON-STATIC
// NEW EVENT DETECTED
bool eventDetected(void) {
	
	eventHandlerStateThisLoop = NO_EVENT_THIS_LOOP;
	
	if (listenNow()) {
		eventHandlerStateThisLoop = newHardwareInputReceived();
	}
	
	return eventHandlerStateThisLoop;
}

// GETTER
bool getEventHandlerStateThisLoop(void) {
	
	return eventHandlerStateThisLoop;
}
