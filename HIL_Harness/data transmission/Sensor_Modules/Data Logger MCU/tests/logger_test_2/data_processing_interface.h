#include <stdint.h>
#include <stddef.h>

#ifndef DATA_PROCESSING_INTERFACE_H
#define DATA_PROCESSING_INTERFACE_H

const size_t LOCAL_ARRAY_SIZE = 5;

extern bool buildLocalArray(void);
extern uint16_t getLocalArray(uint8_t index);
extern uint8_t getSnapshotCounter(void);

#endif
