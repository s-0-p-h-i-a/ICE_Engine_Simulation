/**
ANALOG I/O + DATA LOGGING PROJECT ==============================================================

This project consists of a system with 2 MCUS and I/O peripherals connected via UART:
	- A control MCU that creates output based on user input and periodically sends data to the data logger MCU
	- Data logger MCU receives control system snapshots and processes it for visual display

UART TEST 0 MCU ========================================================================================

This MCU sends a constant integer every 5s. It is meant as a base to verify/validate hardware communication setup.

=================================================================================================

COMPILE:
arduino-cli compile --fqbn arduino:avr:uno test0.ino --clean

arduino-cli upload -p /dev/cu.usbmodem141011 --fqbn arduino:avr:uno test0.ino -v

**/

#include "logger_transmission.h"

void setup(void) {
	
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
}

void loop(void) {
	
	sendDataToLogger();
}
