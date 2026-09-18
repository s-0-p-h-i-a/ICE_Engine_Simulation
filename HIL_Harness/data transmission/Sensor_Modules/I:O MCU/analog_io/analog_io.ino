/**
ANALOG I/O + DATA LOGGING PROJECT ==============================================================

This project consists of a system with 2 MCUS and I/O peripherals connected via UART:
	- A control MCU that creates output based on user input and periodically sends data to the data logger MCU
	- Data logger MCU receives control system snapshots and processes it for visual display

I/O MCU ========================================================================================

This MCU combines analog input from a water level sensor, a joystick and a potentiometer to control an RGB LED.
Each input source controls the brightness of one RGB LED leg. The joystick can either control the red colour brightness or make it blink at full brightness.

It uses input and error handling modules to ensure proper execution.

I/O data is collected and processed for serial monitor display and transmission to a data logger MCU via UART.
Serial monitor visualisation is used to support hw/sw integration and debugging.

See README for more info.

====================================================================================================
COMPILE:
arduino-cli compile --fqbn arduino:avr:uno analog_to_rgb.ino --clean

**/

#include "rgb_led.h"
#include "data_interface.h"
#include "plotter_display.h"
#include "logger_transmission.h"
#include "error_reports.h"

void setup(void) {
	
	pinMode(POTENTIOMETER_PIN, INPUT);
	pinMode(JOYSTICK_X_PIN, INPUT);
	pinMode(WATER_LEVEL_SENSOR_PIN, INPUT);
	
	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinMode(BLUE_PIN, OUTPUT);
}

void loop(void) {
	
	controlRGB();
	getData();
	displayDataSerialPlotter();
	
	if (generateErrorReportArray()) {
		sendDataToLogger();
	} else {
		printErrorReportArrayBuildError();
	}
}
