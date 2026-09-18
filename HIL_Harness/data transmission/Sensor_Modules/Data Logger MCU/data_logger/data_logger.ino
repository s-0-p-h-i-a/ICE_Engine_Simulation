/**
ANALOG I/O + DATA LOGGING PROJECT ==============================================================

This project consists of a system with 2 MCUS and I/O peripherals connected via UART:
	- A control MCU that creates output based on user input and periodically sends data to the data logger MCU
	- Data logger MCU receives control system snapshots and processes it for visual display

Data logger MCU ==================================================================================

This MCU periodically receives 8-bit system data arrays, processes it into a local array and displays the data on the console and on an 8x8 LED matrix.

It receives I/O values and system status reports.

Analog input values from the I/O control MCU are packed into 3 element

See README for more info.

====================================================================================================
COMPILE:
arduino-cli compile --fqbn arduino:avr:uno data_logger.ino

arduino-cli upload -p /dev/cu.usbmodem1101 --fqbn arduino:avr:uno data_logger.ino -v

*/

#include "console_display.h"
#include "matrix_display.h"
#include "system_snapshot.h"
#include <arduino.h>

void setup(void) {
	
	initialiseMatrix();
	Serial.begin(9600);
}

void loop(void) {
	
	readSystemSnapshot();

	displayDataConsole();
	displayDataMatrix();
}
