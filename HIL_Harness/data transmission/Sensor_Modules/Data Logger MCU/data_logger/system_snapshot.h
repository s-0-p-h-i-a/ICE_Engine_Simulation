#include <stdint.h>

#ifndef SYSTEM_SNAPSHOT_H
#define SYSTEM_SNAPSHOT_H

extern void readSystemSnapshot(void);

extern uint16_t getSystemSnapshotData(uint8_t element);

#endif
