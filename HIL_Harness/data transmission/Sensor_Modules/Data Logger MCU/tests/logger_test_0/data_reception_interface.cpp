#include "data_reception_interface.h"
#include <stdint.h>
#include <arduino.h>

static uint16_t systemShapshotCounter = 0;

static byte receivedByte = 0;

static char receivedChar = '0';

static uint8_t receivedInteger = 0;

bool receiveData(void) {

	if(Serial.available() > 0) {
		receivedByte = Serial.read();
		//receivedChar = Serial.read();
		receivedInteger = receivedByte;
		//receivedInteger = receivedChar - '0';
		++systemShapshotCounter;
		return 1;
  } else {
	  return 0;
  }
}

char getReceivedChar(void) {
	return receivedChar;
}

uint8_t getReceivedInteger(void) {
	return receivedInteger;
}

uint8_t getReceivedSnapshotCounter(void) {
	return systemShapshotCounter;
}




