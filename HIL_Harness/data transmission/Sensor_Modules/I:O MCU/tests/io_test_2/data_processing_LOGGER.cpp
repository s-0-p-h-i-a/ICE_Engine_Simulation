#include "data_interface.h"
#include "data_processing_LOGGER.h"
#include <stdint.h>
				
typedef enum {	BITPACKING_HIGH = 0,
				BITPACKING_LOW,
				BITPACKING_FLAG,
				BITPACKING_ELEMENTS } BitpackingElements;
				
static const uint8_t UINT8_MAX_VALUE = 255;

uint8_t transmissionArray[TRANSMISSION_ARRAY_SIZE];

// FORWARD DECLARATIONS
static uint8_t bitPacking16to8_High(uint16_t valueToPack);
static uint8_t bitPacking16to8_Low(uint16_t valueToPack);
static uint8_t bitPacking16to8_Flag(uint16_t valueToPack);

// FUNCTIONS
//// BIT PACKING
////// HIGH
static uint8_t bitPacking16to8_High(uint16_t valueToPack) {
	
	if (valueToPack <= UINT8_MAX_VALUE) {
		return valueToPack;
	} else {
		return valueToPack / UINT8_MAX_VALUE;
	}
}
////// LOW
static uint8_t bitPacking16to8_Low(uint16_t valueToPack) {
	return valueToPack % UINT8_MAX_VALUE;	
}
////// FLAG
static uint8_t bitPacking16to8_Flag(uint16_t valueToPack) {
	
	if (bitPacking16to8_Low(valueToPack) > 0) {
		return 1;
	} else {
		return 0;
	}
}
////// FUNCTIONS ARRAY
static uint8_t (*bitPackingFunctions[BITPACKING_ELEMENTS])(uint16_t valueToPack) =
							{ bitPacking16to8_High, bitPacking16to8_Low, bitPacking16to8_Flag };
	
// ARRAY
bool buildTransmissionArray() {
	
	if (!generateLocalArray()) {
		return 0;
	}
	
	for (int i = 0; i <= TRANSMISSION_ARRAY_SIZE; ++i) {
		
		if (i <= BITPACKING_FLAG) {
			transmissionArray[i] = bitPackingFunctions[i](UINT8_MAX_PLUS_1);
		} else {
			transmissionArray[i] = getLocalArray(i-BITPACKING_FLAG);
		}
	}
	return 1;
}

uint8_t getTransmissionArray(uint8_t arrayIndex) {
	return transmissionArray[arrayIndex];
}
