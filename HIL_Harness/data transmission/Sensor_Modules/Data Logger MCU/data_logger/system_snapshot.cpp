#include "data_processing_interface.h"
#include "system_snapshot.h"
#include <stdint.h>

void readSystemSnapshot(void) {
	
	buildLocalDataArray();
}

uint16_t getSystemSnapshotData(uint8_t element) {
	return getLocalArray(element);
}
