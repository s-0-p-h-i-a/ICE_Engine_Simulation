#include "global.h"
#include "flywheel_lib.h"
#include "engine.h"
#include "drive.h"
#include "joystick.h"
#include "plotter_lib.h"
#include "hall_lib.h"
#include "rpmlib.h"
#include <Arduino.h>

void plotterDisplay(void) {

	Serial.print("RPM:");
	Serial.println(RPM);

	Serial.print("Speed:");
	Serial.println(speed);
	
	Serial.print("Angle:");
	Serial.println(flywheelAngle);
	
	Serial.print("Hall Sensor:");
	Serial.println(hallON);

	Serial.print("X:");
	Serial.print(x);
	
	Serial.print("Engine ON:");
	Serial.print(engineON);
	
	Serial.print("Engine IDLE:");
	Serial.print(isIdle);
	
	//Serial.print(",");
}
