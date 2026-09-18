/*
Local data array = { potentiometer, joystick, water level sensor,
					red brightness, red pause, green brightness, blue brightness,
					system status code, red status code, green status code, blue status code,
					snapshot counter }
*/

#include "data_reception_interface.h"
#include "data_processing_interface.h"
#include <stdint.h>
#include <stddef.h>

typedef enum {	PACKED_DATA_HIGH 	= 0,
				PACKED_DATA_LOW 	= 1,
				PACKED_DATA_FLAG	= 2 } PackedArrayElements;

typedef enum {	POTENTIOMETER 		= 0,
				JOYSTICK			= 1,
				WATER_LEVEL_SENSOR 	= 2,
				RED_BRIGHTNESS		= 3,
				SNAPSHOT_COUNTER	= 17 } InputDataSections;

static const size_t BITPACKED_ARRAY_SIZE = 9;
static const size_t LOCAL_DATA_ARRAY_SIZE = 14;
				
static const uint8_t MAX_VALUE = 255;
static const uint8_t ARRAY_PART2 = 6;
static const uint8_t PACKED_DATA_SECTION = 8;
static const size_t BITPACKED_UNIT_SIZE = 3;

static uint8_t bitPackedDataArray[BITPACKED_ARRAY_SIZE];
static uint16_t localDataArray[LOCAL_DATA_ARRAY_SIZE];

static void buildPackedDataArray(void);
static uint16_t unpackData(uint8_t high, uint8_t low, uint8_t flag);

static void buildPackedDataArray(void) {
	
	for (int i=0; i <= PACKED_DATA_SECTION; ++i) {
		bitPackedDataArray[i] = getReceivedDataArray(i);
	}
}

static uint16_t unpackData(uint8_t high, uint8_t low, uint8_t flag) {
	
	if (flag) {
		return high * MAX_VALUE + low;
	} else {
		return high;
	}
}

void buildLocalDataArray(void) {
	
	receiveDataArray();
	
	buildPackedDataArray();
	
	for (int i = POTENTIOMETER; i <= WATER_LEVEL_SENSOR; ++i) {
		
		uint8_t j = i * BITPACKED_UNIT_SIZE;
		uint8_t k = ++j;
		uint8_t l = ++l;
	
		localDataArray[i] = unpackData(getReceivedDataArray(j), getReceivedDataArray(k), getReceivedDataArray(l));
	}
	
	for (int i = RED_BRIGHTNESS; i < SNAPSHOT_COUNTER; ++i) {
		uint8_t j = i + RED_BRIGHTNESS;
		localDataArray[i] = getReceivedDataArray(j);
	}
	
	localDataArray[SNAPSHOT_COUNTER] = getReceivedSnapshotCounter();
}


uint16_t getLocalArray(uint8_t index) {
	return localDataArray[index];
}
