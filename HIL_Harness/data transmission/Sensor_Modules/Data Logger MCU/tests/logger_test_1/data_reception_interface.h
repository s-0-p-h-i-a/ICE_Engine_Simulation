#include <stdint.h>
#include <stddef.h>

#ifndef DATA_RECEPTION_INTERFACE_H
#define DATA_RECEPTION_INTERFACE_H

static const size_t RECEIVED_ARRAY_SIZE = 5;

extern bool receiveDataArray(void);

extern uint8_t getReceivedDataArray(uint8_t elementNumber);

extern uint8_t getReceivedSnapshotCounter(void);

#endif
