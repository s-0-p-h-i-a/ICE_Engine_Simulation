/*
 22/12
 
CLI FLYWHEEL LOGIC TEST

GOAL:
- Isolate flywheel movement logic and print current angle on terminal to test it independently from servo movement
- First in fixed speed/rpm loop, then with user input for idle rpm or speed 1 or 2
- Next step: integrate it back into arduino sketch and connect it to joystick module for on/off and speed control

SIDE QUEST:
- Refactor present code for 'drive' logic to control the flywheel object via spinflywheel, and the servo will be controlled by spinflywheel
- Change dependencies: currently servolib includes flywheellib, this needs to be reversed
- Reformulated idleCheck logic:
Now:		 	isIdle = engineON && (x < IDLE_INPUT + hyst) && (x > IDLE_INPUT - hyst);
Previously: 	isIdle = engineON && (x < IDLE_INPUT + hyst || x > IDLE_INPUT - hyst);
- Removed isIdle check inside getSpeed function in drive: drive() already runs it
- getSpeedInput previously called in getServoSpeed (now getFlywheelSpeed), now moved to drive() as it is relevant for both the flywheel and the cylinder
- Changed logic inside spinFlywheel:
Now:

	flywheelSpeed = speed;
	flywheelAngle += flywheelSpeed;
	handleEdgeValues();
	checkDirectionToggle();
	moveServo(flywheelAngle);
	
Previously:

	no flywheelSpeed assignment
	flywheelAngle += flywheelSpeed; at the end
- Full engine state check in checkEngineState: engineOn, isIdle and getSpeedInput inside it. Called inside drive()
- Cylinder and flywheel control now directly via spinFlywheel(getFlywheelSpeed()); displayCylinder(getCylinderPause());

- Found issue: flywheelAngle gets stuck at 180, speed toggles but angle isn't decremented
- Terminal output matches servo behaviour, won't sweep back
- Terminal output:

		  Current angle: 178 
		 Current speed: 2 
		  Current angle: 180 
		 Current speed: -2 
		  Current angle: 180 
		 Current speed: -2 
- Likely cause: order of the instructions inside spinFlywheel
- Currently:

		flywheelSpeed = speed;
		flywheelAngle += flywheelSpeed;
	
		handleEdgeValues();
		checkDirectionToggle();
		
- This code:

		flywheelSpeed = flywheelSpeed ? speed : -speed;
	
		handleEdgeValues();
		checkDirectionToggle();
		flywheelAngle += flywheelSpeed;
		
- Prints: 
		 Current angle: 178 
		 Current speed: 2 
		  Current angle: 180 
		 Current speed: 2 
		  Current angle: 178 
		 Current speed: -2 
		  Current angle: 180 
		 Current speed: 2 
		  Current angle: 178 
		 Current speed: -2 

- Difficulty: finding a logic that works both on the very first run and across loops
- Sticking point: initial (first loop) assignment of flywheelSpeed
	-> initialised speed at 0 causes issues with control branches
- Sweep back logic fixed for when angle hits 180, now gets stuck at -2 and -2
- Added sweepBack check in check...Toggle so it gets updated there too
- Issue solved! flywheelSpeed assignment in main spinflywheel, after sweepback check, separated from sweep back concern


 */


#include "drive.h"
#include "flywheel_libTEST.h"
#include <stdio.h>

int main(void) {
	
	checkEngineState();
	
	bool runLoop = 1;
	
	while (runLoop) {

		printf("Current angle: %d \n", flywheelAngle);
		printf(" ");
		printf("Current speed: %d \n", flywheelSpeed);
		printf(" ");
		printf(" ");
		driveEngine();
		++currentLoop;
		runLoop = !getEngineStateCheckNow();
	}
	return 0;
}
