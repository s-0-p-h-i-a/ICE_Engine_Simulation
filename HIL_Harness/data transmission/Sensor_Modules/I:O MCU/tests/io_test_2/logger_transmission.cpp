#include "data_processing_LOGGER.h"
#include "logger_transmission.h"
#include <stdint.h>
#include <arduino.h>
				
static const uint16_t TRANSMISSION_PAUSE = 5000;
static const uint8_t LAST_ELEMENT_POSITION = 6;

static unsigned long lastTransmissionTime = 0;

static bool checkIfPauseElapsed(void);

static bool checkIfPauseElapsed(void) {
	
	unsigned long timeNow = millis();
	
	unsigned long delta = timeNow - lastTransmissionTime;
	
	if (delta > TRANSMISSION_PAUSE) {
		lastTransmissionTime = timeNow;
		return 1;
	} else {
		return 0;
	}	
}

bool sendDataToLogger(void) {
	
	if (!(checkIfPauseElapsed() && buildTransmissionArray())) {
		return 0;
	}
	
	Serial.write((byte*)transmissionArray, TRANSMISSION_ARRAY_SIZE * sizeof(transmissionArray[0]));
	return 1;
}


/**bool sendDataToPlotter(void) {
	
	if (!(checkIfPauseElapsed() && buildTransmissionArray())) {
		return 0;
	}
	
	Serial.println(" ");
	Serial.print("System snapshot number: ");
	Serial.print(snapshotCounter);
	Serial.println(" ");
	Serial.print("Transmitted array: ");
	Serial.print("{ ");
	
	for (int i = 0; i < TRANSMISSION_ARRAY_SIZE; ++i) {
		Serial.print(getTransmissionArray(i));
		if ( i != LAST_ELEMENT_POSITION) {
			Serial.print(", ");
		}
	}
	Serial.print(" }");
	Serial.println(" ");
	
	return 1;
}**/
