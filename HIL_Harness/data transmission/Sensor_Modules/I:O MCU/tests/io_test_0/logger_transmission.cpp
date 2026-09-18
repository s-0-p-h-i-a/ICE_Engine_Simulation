#include "logger_transmission.h"
#include <stdint.h>
#include <arduino.h>
				
static const uint16_t TRANSMISSION_PAUSE = 5000;

static const byte transmissionByte = 0x1;

//static const char transmissionChar = '1';

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


void sendDataToLogger(void) {
	if (checkIfPauseElapsed()) {
		Serial.write(transmissionByte);
		digitalWrite(LED_BUILTIN, HIGH);
		//Serial.println(transmissionByte);
	}
}


/**
FOR LOCAL SERIAL PLOTTER CHECK

static uint16_t systemShapshotCounter = 0;

void printData(void) {

	if (checkIfPauseElapsed()) {
		Serial.print(" ");
		Serial.print("Snapshot nr: ");
		Serial.print(systemShapshotCounter);
		Serial.print(" ");
		Serial.print("Data: ");
		Serial.println(transmissionData);
	}
}
**/
