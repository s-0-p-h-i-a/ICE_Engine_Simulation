#include <stdint.h>

#ifndef DATA_RECEPTION_INTERFACE_H
#define DATA_RECEPTION_INTERFACE_H

extern void receiveDataArray(void);

extern uint8_t getReceivedDataArray(uint8_t elementNumber);

extern uint8_t getReceivedSnapshotCounter(void);

#endif
