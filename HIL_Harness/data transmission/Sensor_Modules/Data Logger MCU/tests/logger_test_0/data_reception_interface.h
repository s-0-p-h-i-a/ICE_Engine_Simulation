#include <stdint.h>

#ifndef DATA_RECEPTION_INTERFACE_H
#define DATA_RECEPTION_INTERFACE_H

extern bool receiveData(void);

extern char getReceivedChar(void);
extern uint8_t getReceivedInteger(void);
extern uint8_t getReceivedSnapshotCounter(void);

#endif
