/*
------------------------------------------------------------------------------
Project: Mini Engine / RPM Simulation — v0

Summary:
Initial end-to-end implementation of a joystick-controlled engine simulation,
including flywheel + servo actuation, hall sensor RPM measurement, cylinder
state machine, and LED-based visualization.
------------------------------------------------------------------------------
Key Work Done:
- Implemented engine drive logic (on/off, idle, speed levels via joystick)
- Built independent cylinder and flywheel state machines
- Integrated servo + hall sensor for RPM calculation
- Isolated timing logic per module using millis()
- Reduced global state and clarified module ownership
------------------------------------------------------------------------------
Notable Debug Example:
Flywheel sweep-back bug discovered via serial output:
    Current angle: 178 | speed:  2
    Current angle: 180 | speed:  2
    Current angle: 178 | speed: -2
    Current angle: 180 | speed:  2

Root causes:
- Unsigned angle type breaking decrement logic
- Direction toggling split across multiple functions

Fix:
- Centralised flywheel angle + speed updates
- Separated edge detection, direction toggle, and angle increment
- Corrected data types and initialisation order
------------------------------------------------------------------------------
Status at End of v0:
- Engine, cylinder, flywheel, servo, and joystick logic validated
- RPM system rebuilt with clearer hall/RPM separation
- Architecture ready for data interfaces and further refactors (v1)
------------------------------------------------------------------------------
 CLI Compile & Upload:
 arduino-cli compile --fqbn arduino:avr:uno V0.ino
 arduino-cli upload -p /dev/cu.usbmodem141011 --fqbn arduino:avr:uno
 ------------------------------------------------------------------------------
 */

#include "joystick.h"
#include "drive.h"
#include "cylinderVisuals.h"
#include "servolib.h"
#include "rpmlib.h"
#include "plotter.h"

void setup() {

	Serial.begin(9600);
	
	pinMode(JOYSTICK_Z,INPUT);
	
	serv0.attach(SERVO_PIN);
	
	startEngineVisuals();
}

void loop() {
	
	driveEngine();
	plotterDisplay();
	getRPM();
}
