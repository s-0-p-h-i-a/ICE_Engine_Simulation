/**
ANALOG I/O + DATA LOGGING PROJECT ==============================================================

This project consists of a system with 2 MCUS and I/O peripherals connected via UART:
	- A control MCU that creates output based on user input and periodically sends data to the data logger MCU
	- Data logger MCU receives control system snapshots and processes it for visual display

Data logging MCU  - Test 0 =====================================================================

This MCU sends a constant integer to the receiver MCU.
This test isolates all other mechanisms in the I/O + data logging system project and simply focuses on validating UART data transmission.

See README for more info.

====================================================================================================

COMPILE:
arduino-cli compile --fqbn arduino:avr:uno logger_test_0.ino

arduino-cli upload -p /dev/cu.usbmodem1101 --fqbn arduino:avr:uno logger_test_0.ino -v

*/

#include "console_display.h"
#include "data_reception_interface.h"
#include <arduino.h>

void setup(void) {
	
	Serial.begin(9600);
}

void loop(void) {
	
	if (receiveData()) {
	
		displayDataConsole();
	}
}
