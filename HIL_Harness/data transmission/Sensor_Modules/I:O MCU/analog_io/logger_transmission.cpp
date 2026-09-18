#include "data_processing_LOGGER.h"
#include "logger_transmission.h"
#include <stdint.h>
#include <arduino.h>
				
typedef enum { 	BATCH_1_END 	= 2, // potentiometer
				BATCH_2_START 	= 3, // joystick
				BATCH_2_END 	= 5,
				BATCH_3_START	= 6, // water level sensor
				BATCH_3_END		= 8,
				BATCH_4_START	= 9, // red brightness, red pause, green brightness, blue brightness
				BATCH_4_END		= 12,
				BATCH_5_START	= 13, // joystick system status, joystick system error counter
				BATCH_5_END		= 18 } TransmissionArraySections;
				
static const uint16_t TRANSMISSION_PAUSE = 5000;

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

	if (checkIfPauseElapsed() && packTransmissionArray()) {
		Serial.write((byte*)transmissionDataArray, TRANSMISSION_ARRAY_SIZE * sizeof(transmissionDataArray[0]));
		return 1;
	}
	return 0;
}
