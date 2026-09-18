#include "data_reception_interface.h"
#include "console_display.h"
#include <arduino.h>

static const uint8_t LAST_ELEMENT_POSITION = 4;

void displayDataConsole(void) {
	
	Serial.println(" ");
	Serial.print("System snapshot number: ");
	Serial.print(getReceivedSnapshotCounter());
	Serial.println(" ");
	Serial.print("Received array: ");
	Serial.print("{ ");
	
	for (int i = 0; i < RECEIVED_ARRAY_SIZE; ++i) {
		Serial.print(getReceivedDataArray(i));
		if ( i != LAST_ELEMENT_POSITION) {
			Serial.print(", ");
		}
	}
	Serial.print(" }");
	Serial.println(" ");
}
	
