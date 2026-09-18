#include "data_reception_interface.h"
#include <stdint.h>
#include <stddef.h>
#include <arduino.h>

static uint16_t systemShapshotCounter = 0;

static uint8_t receivedDataArray[RECEIVED_ARRAY_SIZE];

bool receiveDataArray(void) {

	if(Serial.available() >= RECEIVED_ARRAY_SIZE * sizeof(receivedDataArray[0])) {
		Serial.readBytes((byte*)receivedDataArray, RECEIVED_ARRAY_SIZE * sizeof(receivedDataArray[0]));
		++systemShapshotCounter;
		return 1;
	}
  return 0;
}

uint8_t getReceivedDataArray(uint8_t elementNumber) {
	return receivedDataArray[elementNumber];
}

uint8_t getReceivedSnapshotCounter(void) {
	return systemShapshotCounter;
}



