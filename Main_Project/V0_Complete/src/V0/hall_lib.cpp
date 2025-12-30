/*
 HALL + RPM LIBS REWRITE
 
 Structure choice:
 - Better separation of concerns, less passing variables back and forth.
 - Move from void functions that affect global vars to return functions to feed temporary data to RPM calculation 
 
 PROCESS:
 - Start in rpmlib, find global vars from hall lib being used, switch for return funcs
 - Central timing inside hall lib only, no 'now' usage in rpmlib
 - Move getTimeWindow to hall lib? Hall does own time and the time window is data for rpm calc, not rpm calc itself
 - Idea: gettimewindow as 'api call' for rpmlib, rpmlib pure rpm math from hall lib defined timing, no delta calc in rpm
 - Right now rpmlib includes drive, but that doesnt make sense
 - Currently rpm calculation checks if the engine is on, but this doesn't need to be done as rpm = 0 from no flywheel movement can just be baked into overall rpm calc, ie if there are no sensor reads the deltas are 0
 - Considered making rpm unsigned now, but interested to see if any weird logic bug leads to a negative rpm, once all fully validated will make it unsigned
 - Same for gettime window
 - RPM lib now only accesses data from hall lib via readhallnow and gettimewindow
 - Hall state needs to be updated at every loop, so function calls for that need to happen somewhere that always runs
 - The reason why constant hall updates are needed is specifically for rpm calc. rpmlib already includes halllib, and getrpm runs every loop: run hall state check inside getrpm as that is what it is needed for
 - Read hall now = hall on and !secondpass
 - All vars in hall lib now static
 - Hall pin enum now also static, add pinmode to inithall
 - RPM calc: no more global rpm var, now getrpm returns value
 - inithall redundant as vars are already initialised, remove and just pinmode in main
 - What to do about now? Updatehall state gets now from millis at every loop, gettimewindow then uses that now in rpm calculation: the now from updatehall and gettimewindow come from the same loop
 - Secondpass logic seems weird?
 - Idea for secondpass logic: hall does not need to know the exact current flywheel angle, it just know it alternates between coming & going
 - secondpass starts at 0 and toggles at every sensor pass = moving forward or backward
 - thisrpm and lastrpm are both raw punctual rpm snapshots: give current averaged rpm to thisrpm?
 - Pros: smoother value, maybe better for slow movement like servo
	Cons: delayed, less detailed representation?
- Smoother might be more visually coherent for this?

HW INTEGRATION:
- Serial plotter test shows secondpass constantly toggling back and forth by continuous hall sensor activation
- How to handle non moving constant sensor input inside hall lib?
- Fully independent or includes drive to know if engine is on or not, only calculates if it is?
- Add debounce to read?
- Added lasthallstate check: now secondpass doesnt jitter anymore, but it doesnt toggle either
- Fixed if condition: now no jitter and toggles
- New issue found: rpm stuck at 1
- Reduced getrpm to just constant rpm calc, no hall status check. that goes in gettimewindow now
- RPM sometimes randomly goes to 1, then back to 0 if the hall sensor passes again
- If i do nothing (no hall sensor activation), rpm goes up by 1 every minute
- Potential rounding down issue causing 0 output?
- Changed logic in updatehallstate, now sensor read is only updated at every second pass (math) and secondpass toggles each time hall is on
- Weird stuff happening with in/double/round

 
 
 */

#include "drive.h"
#include "hall_lib.h"
#include <Arduino.h>
#include <stdint.h>

#define ZERO 0

uint8_t hallON = 0;
uint8_t lastHallState = 0;
uint8_t secondPass = 0;

static long unsigned int currentSensorRead = ZERO;
static long unsigned int lastSensorRead = ZERO;
static long unsigned int now = ZERO;

void updateHallState(void) {

	lastHallState = hallON;
	hallON = !digitalRead(HALL_PIN);
	now = millis();
	
	if (hallON && !lastHallState) {
		if (!secondPass) {
		
			lastSensorRead 		= currentSensorRead;
			currentSensorRead = now;
		}
		secondPass = !secondPass;
	}
}

unsigned long int getTimeWindow(void) {

	unsigned long int timeWindow = now - lastSensorRead;

	return timeWindow;
}