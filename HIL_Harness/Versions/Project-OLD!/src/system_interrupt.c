#include "system_interrupt.h"
#include <stdint.h>
#include <stdbool.h>

// CONSTANTS
static const uint8_t TIMER_ZERO = 0;
static const uint8_t TIMER_PHASE = 50;

// VARIABLES
static uint8_t interruptTimer = TIMER_ZERO;

// STATIC FUNCTIONS
static bool updateInterruptTimer(void);
static bool validReadWindow(void);

static bool updateInterruptTimer(void) {
	
	unsigned long timeNow = HAL_GetTick();
	
	interruptTimer = timeNow % TIMER_PHASE;
	
	return 1;
}

static bool validReadWindow(void) {
	return interruptTimer == TIMER_ZERO;
}

// FUNCTIONS
bool interruptDetected(void) {
	
	updateInterruptTimer();
	
	if (validReadWindow()) {
		return newHardwareInputReceived();
	}
	return 0;
}

bool resetInterruptCounter(void) {
	interruptTimer = TIMER_ZERO;
	return 1;
}


/**It has its own timer that 'listens' every 50ms. Lower interrupt listen frequency intentionally chosen to make system less responsive and allow user to move more slowly through speed/braking range while visualising data on serial plotter.**/
