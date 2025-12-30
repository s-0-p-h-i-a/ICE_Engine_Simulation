/*
 
 arduino-cli compile --fqbn arduino:avr:uno 1cylinder_v0.ino

08/12 FIRST DRAFT REWORK

MAIN CHANGES

- Converting enums to static const int to keep them contained and avoid enum confusion

- Increasing overall "containment" of variables, functions, function calls

- Swapping some variables for getVariable function calls where appropriate (= values needed as intermediary or to be checked punctually, they do not need to exist between loops)

- Using local intermediary variables

- Clearer conceptual separation: cylinder and flywheel are now separate parts of a composite engine multi state machine

- Component states are now static local variables: the state of the cylinder (which stroke it is on) depends on the state of the engine, but the engine does not need to know the state of the cylinder to assess its own state (defined only by latched button input)

- Cylinder display now uses millis() call instead of global int now to control on/off timing: the blinking timing is its own independent state separate from the rest of the system

- Joystick debounce now also uses a millis() variable for its debounce tracking

- Now only cylinder display, joystick and drive modules need the current time. They now all use their town millis() calls for independent time tracking

- Most global enums changed for local static const int or #defines

 */

#include "joystick.h"
#include "drive.h"
#include "cylinderVisuals.h"
#include "plotter.h"

void setup() {

	Serial.begin(9600);
	
	pinMode(JOYSTICK_X,INPUT);
	pinMode(JOYSTICK_Z,INPUT);
	
	pinMode(intakeValvePin,OUTPUT);
	pinMode(exhaustValvePin,OUTPUT);
	pinMode(fuelInjectorPin,OUTPUT);
	pinMode(sparkPlugPin,OUTPUT);
	pinMode(tdcPin1,OUTPUT);
	pinMode(tdcPin2,OUTPUT);
	pinMode(bdcPin1,OUTPUT);
	pinMode(bdcPin2,OUTPUT);
}

void loop() {
	
	driveEngine();

	plotterDisplay();
}
