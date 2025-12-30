#include "drive.h"
#include "cylinder.h"
#include "flywheel_lib.h"
#include "joystick.h"
#include "hall_lib.h"
#include "rpmlib.h"
#include "plotter.h"
#include <arduino.h>

void plotterDisplay(void) {
	Serial.print("Engine_ON:");
	Serial.print(engineON+50);
	Serial.print(",");
	Serial.print("Engine_Idle:");
	Serial.print(isIdle+70);
	Serial.print(",");
	Serial.print("Speed_One:");
	Serial.print(speedOneInput*20);
	Serial.print(",");
	Serial.print("Speed_Two:");
	Serial.print(speedTwoInput*20);
	Serial.print(",");
	/*Serial.print(flywheelAngle);
	Serial.print(" ");
	Serial.print(flywheelSpeed*10);
	Serial.print(" ");
	Serial.print(cylinderStateNow*100 + 50);;
	Serial.print(" ");
	Serial.print(readJoystick_Z()*100);
	Serial.print(" ");
	Serial.print(readJoystick_X());
	**/
	Serial.print("Hall_Sensor:");
	Serial.print(hallON*50);
	Serial.print(",");
	Serial.print("Second_Pass:");
	Serial.print(secondPass*70);
	Serial.print(",");
	Serial.print("RPM:");
	Serial.println(getRPM()*100);
}
