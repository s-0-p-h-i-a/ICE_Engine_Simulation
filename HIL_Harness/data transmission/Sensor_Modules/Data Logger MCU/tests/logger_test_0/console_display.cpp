#include "data_reception_interface.h"
#include "console_display.h"
#include <stdint.h>
#include <arduino.h>

void displayDataConsole(void) {
	
	Serial.print("System snapshot number: ");
	Serial.print(getReceivedSnapshotCounter());
	Serial.println(" ");
	Serial.print("Received data: ");
	Serial.println(getReceivedInteger());
	Serial.println(" ");
}
