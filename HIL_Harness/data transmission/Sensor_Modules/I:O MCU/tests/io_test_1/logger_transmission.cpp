#include "logger_transmission.h"
#include <stdint.h>
#include <arduino.h>

const size_t TRANSMISSION_ARRAY_SIZE = 5;
uint8_t transmissionDataArray[TRANSMISSION_ARRAY_SIZE];
				
static const uint16_t TRANSMISSION_PAUSE = 5000;

static unsigned long lastTransmissionTime = 0;

static bool checkIfPauseElapsed(void);
bool packTransmissionArray(void);

bool packTransmissionArray(void) {
	
	for (int i = 0; i < TRANSMISSION_ARRAY_SIZE; ++i) {
		
		transmissionDataArray[i] = 1;
	}
	return 1;
}

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

	if (checkIfPauseElapsed() && packTransmissionArray()) {
		Serial.write((byte*)transmissionDataArray, TRANSMISSION_ARRAY_SIZE * sizeof(transmissionDataArray[0]));
		return 1;
	}
	return 0;
}
