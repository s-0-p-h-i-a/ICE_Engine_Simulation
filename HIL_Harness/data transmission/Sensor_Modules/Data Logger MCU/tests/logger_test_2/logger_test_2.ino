/**
ANALOG I/O + DATA LOGGING PROJECT ==============================================================

This project consists of a system with 2 MCUS and I/O peripherals connected via UART:
	- A control MCU that creates output based on user input and periodically sends data to the data logger MCU
	- Data logger MCU receives control system snapshots and processes it for visual display

I/O MCU  - Test 2 =====================================================================

This MCU receives a constant array[7] of 1's from the transmitter MCU. It then processes it into a local 5 element array.

The first 3 elements of the received array represent a bit-packed uint16 integer x = 256:
	- 1st element: x / 255 = 1
	- 2nd element: x % 255 = 1
	- 3rd element: boolean flag indicating whether uint16 x had to be bit-packed for uint8 representation
		-> if yes: = 1
		
The data logger output is then: { 256, 1, 1, 1, 1 }

This test isolates all other mechanisms in the I/O + data logging system project and simply focuses on validating UART data transmission.

See README for more info.

=================================================================================================

COMPILE:
arduino-cli compile --fqbn arduino:avr:uno logger_test_2.ino --clean

arduino-cli upload -p /dev/cu.usbmodem1101 --fqbn arduino:avr:uno logger_test_2.ino -v

(elegoo board)

**/

#include "data_reception_interface.h"
#include "console_display.h"
#include <arduino.h>

void setup(void) {
	
	Serial.begin(9600);
}

void loop(void) {
	
	if (receiveArray()) {
	
		displayDataConsole();
	}
}
