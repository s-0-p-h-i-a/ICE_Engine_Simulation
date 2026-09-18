#include "data_interface.h"
#include <stdint.h>
#include <stddef.h>

static const size_t LOCAL_ARRAY_SIZE = 5;

static uint16_t localArray[LOCAL_ARRAY_SIZE] = { 0, 0, 0, 0, 0 };

static bool checkIfArrayGenerated(LocalArrayElements arrayIndex);

static bool checkIfArrayGenerated(LocalArrayElements arrayIndex) {
	
	if ((arrayIndex == POS0) && (localArray[arrayIndex] != UINT8_MAX_PLUS_1)) {
			return 0;
		}
	
	if (localArray[arrayIndex] != ARRAY_CONST_VALUE) {
		return 0;
	}
	
	return 1;
}

bool generateLocalArray(void) {
	
	localArray[POS0] = UINT8_MAX_PLUS_1;
	
	for (int i = POS1; i <= POS4; ++i) {
		
		localArray[i] = ARRAY_CONST_VALUE;
		
		if (!checkIfArrayGenerated(i)) {
		return 0;
		}
	}
	
	return 1;
}

bool getLocalArray(LocalArrayElements arrayIndex) {
	return localArray[arrayIndex];
}
