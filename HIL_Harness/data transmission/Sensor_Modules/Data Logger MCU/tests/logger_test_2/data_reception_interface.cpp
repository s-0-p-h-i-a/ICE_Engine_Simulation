#include "data_reception_interface.h"
#include <stdint.h>
#include <stddef.h>
#include <arduino.h>

static const size_t RECEIVED_ARRAY_SIZE = 7;

static uint16_t systemShapshotCounter = 0;

static uint8_t receivedArray[RECEIVED_ARRAY_SIZE];

bool receiveArray(void) {

	if(Serial.available() >= RECEIVED_ARRAY_SIZE * sizeof(receivedArray[0])) {
		Serial.readBytes((byte*)receivedArray, RECEIVED_ARRAY_SIZE * sizeof(receivedArray[0]));
		++systemShapshotCounter;
		return 1;
	}
	
  return 0;
}

uint8_t getReceivedArray(uint8_t elementNumber) {
	return receivedArray[elementNumber];
}

uint8_t getReceivedSnapshotCounter(void) {
	return systemShapshotCounter;
}



