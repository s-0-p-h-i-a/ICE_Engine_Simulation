#include "data_processing_PLOTTER.h"
#include "plotter_display.h"
#include <arduino.h>


void displayDataSerialPlotter(void) {
	
	computePlotterValues();
	
	Serial.print("Potentiometer:");
	Serial.print(",");
	Serial.print(potentiometer_Input_PLOTTER);
	Serial.print(",");
	
	Serial.print("Joystick:");
	Serial.print(",");
	Serial.print(joystick_Input_PLOTTER);
	Serial.print(",");
	
	Serial.print("Water_Level:");
	Serial.print(",");
	Serial.print(potentiometer_Input_PLOTTER);
	Serial.print(",");
	
	Serial.print("RED_brightness:");
	Serial.print(",");
	Serial.print(redBrightnessLevel_PLOTTER);
	Serial.print(",");
	
	Serial.print("RED_pause:");
	Serial.print(",");
	Serial.print(redPauseTime_PLOTTER);
	Serial.print(",");
	
	Serial.print("GREEN_brightness:");
	Serial.print(",");
	Serial.print(greenBrightnessLevel_PLOTTER);
	Serial.print(",");
	
	Serial.print("BLUE_brightness:");
	Serial.print(",");
	Serial.print(blueBrightnessLevel_PLOTTER);
	Serial.println(",");	
}
