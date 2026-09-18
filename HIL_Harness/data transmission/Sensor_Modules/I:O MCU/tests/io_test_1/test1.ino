/**
ANALOG I/O + DATA LOGGING PROJECT ==============================================================

This project consists of a system with 2 MCUS and I/O peripherals connected via UART:
	- A control MCU that creates output based on user input and periodically sends data to the data logger MCU
	- Data logger MCU receives control system snapshots and processes it for visual display

I/O MCU  - Test 1 =====================================================================

This MCU sends a constant array[5] of 1's to the receiver MCU.

This test isolates all other mechanisms in the I/O + data logging system project and simply focuses on validating UART data transmission.

See README for more info.

=================================================================================================

COMPILE:
arduino-cli compile --fqbn arduino:avr:uno test1.ino --clean

arduino-cli upload -p /dev/cu.usbmodem141011 --fqbn arduino:avr:uno test1.ino -v

**/

#include "logger_transmission.h"

void setup(void) {
	
	Serial.begin(9600);
}

void loop(void) {
	
	sendDataToLogger();
}
