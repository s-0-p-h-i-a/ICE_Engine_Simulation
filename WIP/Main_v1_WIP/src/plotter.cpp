#include "engine_system_data.h"
#include "rpm_system_data.h"
#include "plotter.h"
#include <arduino.h>

void updatePlotterValues(void) {
	updateRpmSystemValues();
	updateEngineSystemValues();
}

void plotterDisplay(void) {
	
	updatePlotterValues();
	
	Serial.print("Engine_ON:");
	Serial.print(plotter_engineON);
	Serial.print(",");
	Serial.print("Engine_Idle:");
	Serial.print(plotter_isIdle);
	Serial.print(",");
	Serial.print("Speed_One:");
	Serial.print(plotter_speedOne);
	Serial.print(",");
	Serial.print("Speed_Two:");
	Serial.print(plotter_speedTwo);
	Serial.print(",");
	Serial.print("Flywheel_Angle:");
	Serial.print(plotter_FlywheelAngle);
	Serial.print(",");
	Serial.print("Cylinder_State:");
	Serial.print(plotter_cylinderState);
	Serial.print(",");
	Serial.print("Hall_Sensor:");
	Serial.print(plotter_HallState);
	Serial.print(",");
	Serial.print("Second_Pass:");
	Serial.print(plotter_HallSecondPass);
	Serial.print(",");
	Serial.print("RPM:");
	Serial.println(plotter_RPM);
}
