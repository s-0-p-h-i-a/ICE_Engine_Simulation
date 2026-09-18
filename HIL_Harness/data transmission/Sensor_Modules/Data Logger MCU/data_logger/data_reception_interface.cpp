#include "data_reception_interface.h"
#include <stdint.h>
#include <stddef.h>
#include <arduino.h>

static const size_t DATA_ARRAY_SIZE = 19;

static uint16_t systemShapshotCounter = 0;

static uint8_t receivedDataArray[DATA_ARRAY_SIZE];

void receiveDataArray(void) {

	if(Serial.available() >= DATA_ARRAY_SIZE * sizeof(receivedDataArray[0])) {
		Serial.readBytes((byte*)receivedDataArray, DATA_ARRAY_SIZE * sizeof(receivedDataArray[0]));
		++systemShapshotCounter;
  }
}

uint8_t getReceivedDataArray(uint8_t elementNumber) {
	return receivedDataArray[elementNumber];
}

uint8_t getReceivedSnapshotCounter(void) {
	return systemShapshotCounter;
}



