/*
 
 arduino-cli compile --fqbn arduino:avr:uno V1.ino
 
 arduino-cli upload -p /dev/cu.usbmodem141011 --fqbn arduino:avr:uno .


26/12
V0.1: Semantic Rework and Code Clean-Up

Semantics:
- Renaming vars/funcs in starting in flywheel lib (now flywheel ref)
- Goal: improved representation of actions/roles, better readability

Clean-Up:
- Making helper funcs static, tidying up flag updates, tightening up separation of concerns+encapsulation
- Flywheel ref lib: removed changedirection from clampangle, now only updated in updatedirection
- Defining accessory funcs to feed modules data for visualisation
- Implementing 2 system data interface modules: collect data from modules, process for visualisation, feed to plotter display module


LOGIC CHANGE: FLYWHEEL+SERVO
- Flywheel object now "spins" = angle oscillates between 0° and 360°
- Servo module interprets > 180° angle input by clamping servo to 0°-180° and reversing direction

30/12
CRANKSHAFT MODULE ADDITION
STRUCTURAL CHANGE: new engine module containing cylinder + crank

- Engine module = interface between drive and cylinder+crank

Drive module changes:

- Remove assignCylinderBlinkPause: will now happen in engine module
- Write assignEngineSpeed: feeds speed level to engine module to control cylinder and crank

- Semantics note: closest physical equivalent to user speed level input is throttle input.
Rename speed level to throttle level later?

- New idea: in real life the flywheel is moved by the crank. make flywheel be actuated by the crank?
- Currently drive.cpp both controls and represents the engine: make it a pure control interface, engine state goes in engine module
- Void funcs in drive -> return funcs for engine state
- Include drive in engine or vice versa? Which will own the function that runs in void loop?
- Engine module needs drive to translate user input into speed settings -> engine includes drive
- Engine is the main 'object' and joystick+drive are accessories to control it
- Drive module 'does', engine module 'is'
- Throttle level state lives in drive
- Drive now pure user input processing layer: provides bool turnEngineOnOff and unsigned int assignThrottleLevel results to engine module
- All flywheel control logic moved to crankshaft
- Throttle level from drive module used to index states array in engine
- Crank controls flywheel, but what exactly is crank speed?
	-> Define crank speed (=response to throttle input) as rotational speed, eg current angle step logic from flywheel.
	- Now crank spins and flywheel 'interface' now just responds by translating it to servo movement
- Bit of redundancy between the throttle level bool variables in drive and the engine speed state in engine but more explicit/verbose separation between input states and rotational speed levels was chosen to help with reasoning while building
- Modular structure made it possible to implement these changes while keeping some relevant modules intact (eg cylinder), and translating code from one to another easily with copy/paste and semantic changes (flywheel logic -> crank logic)
- Intentional encapsulation: aiming for clean #include structuring and logical layering

Current files structure

* Main calls driveEngine, getRPM and plotterDisplay
	- driveEngine: in engine.cpp
	
	Calls: 	- turnEngineOnOff and assignThrottleLevel from drive.cpp for input ('sideways' branch)
	
			- advanceCrankshaftAngle and displayCylinder for action output
			
				- advanceCrankshaftAngle: in crankshaft.cpp
				Calls: advanceFlywheelAngle
				
					- advanceFlywheelAngle: in flywheel_ref.cpp
					Calls: moveServo from servo.cpp 
					
				- displayCylinder: in cylindervisuals.cpp
				Calls: updateCylinderState
				
					- updateCylinderState: in cylinder.cpp
					
	- getRPM: calls updateHallState and getTimeDelta from hall.cpp
	
	- plotterDisplay: calls functions from data interfaces
	
- New change: crank controls cylinder blink? -> centralised blink pause and step angle control inside crank
- Moving blink timing logic up from cylindervisuals to crankshaft? cylindervisuals and crankvisuals use the same timing
- Crank/crank timing/cylinder display/crank display uses centralised "stroke" state: corresponds to state of LED cylinder and of lobe 1 in the virtual crank
- Virtual crank object logic assumes 1342 firing order
- Main crankshaft module moves both the servo 'flywheel' and cylinder+crank LED display
- Crankshaft lobes representation currently limited to crankshaftvisuals
					

 */

#include "joystick.h"
#include "drive.h"
#include "cylinderVisuals.h"
#include "servolib.h"
#include "rpmlib.h"
#include "plotter.h"

void setup() {

	Serial.begin(9600);
	initJoystick();
	initServoSystem();
	initCylinderVisuals();
}

void loop() {
	
	driveEngine();
	getRPM();
	plotterDisplay();
}
