#include "data_reception_interface.h"
#include "data_processing_interface.h"
#include <stdint.h>
#include <stddef.h>

// CONSTANTS
typedef enum {	PACKED_DATA_HIGH 	= 0,
				PACKED_DATA_LOW 	= 1,
				PACKED_DATA_FLAG	= 2,
				PACKED_ELEMENTS } BitpackingElements;	

static const size_t BITPACKING_ARRAY_SIZE = 3;
static const uint8_t FIRST_ARRAY_POSITION = 0;
static const uint8_t NON_PACKED_ELEMENTS_POSITION = 1;
static const uint8_t UINT8_MAX_VALUE = 255;

// ARRAYS
static uint16_t localArray[LOCAL_ARRAY_SIZE] = { 0, 0, 0, 0, 0 };
static uint8_t bitpackedArray[BITPACKING_ARRAY_SIZE];

// STATIC FUNCTIONS
static uint16_t unpackData(uint8_t packedData[BITPACKING_ARRAY_SIZE]);

static uint16_t unpackData(uint8_t packedData[BITPACKING_ARRAY_SIZE]) {
	
	if (packedData[PACKED_DATA_FLAG]) {
		return packedData[PACKED_DATA_HIGH] * UINT8_MAX_VALUE + packedData[PACKED_DATA_LOW];
	} else {
		return packedData[PACKED_DATA_HIGH];
	}
}

// GLOBAL FUNCTIONS
bool buildLocalArray(void) {
	
	for (int i = PACKED_DATA_HIGH; i <= PACKED_DATA_FLAG; ++i) {
		bitpackedArray[i] = getReceivedArray(i);
	}
	
	localArray[FIRST_ARRAY_POSITION] = unpackData(bitpackedArray);

	for (int i = NON_PACKED_ELEMENTS_POSITION; i < LOCAL_ARRAY_SIZE; ++i) {
		uint8_t j = i + NON_PACKED_ELEMENTS_POSITION;
		localArray[i] = getReceivedArray(j);
	}
	return 1;
}

uint16_t getLocalArray(uint8_t index) {
	return localArray[index];
}

uint8_t getSnapshotCounter(void) {
	return getReceivedSnapshotCounter();
}
