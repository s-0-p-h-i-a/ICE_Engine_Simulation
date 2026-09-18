#include "data_processing_interface.h"
#include "console_display.h"
#include <arduino.h>

static const uint8_t LAST_ELEMENT_POSITION = 4;

bool displayDataConsole(void) {
	
	if (!buildLocalArray()) {
		return 0;
	}
	
	Serial.println(" ");
	Serial.print("System snapshot number: ");
	Serial.print(getSnapshotCounter());
	Serial.println(" ");
	Serial.print("Received array: ");
	Serial.print("{ ");
	
	for (int i = 0; i < LOCAL_ARRAY_SIZE; ++i) {
		Serial.print(getLocalArray(i));
		if ( i != LAST_ELEMENT_POSITION) {
			Serial.print(", ");
		}
	}
	Serial.print(" }");
	Serial.println(" ");
	
	return 1;
}
	
