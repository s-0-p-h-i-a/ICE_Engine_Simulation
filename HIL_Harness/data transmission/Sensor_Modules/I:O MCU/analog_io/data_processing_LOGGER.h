#include <stddef.h>
#include <stdint.h>

#ifndef DATA_PROCESSING_LOGGER_H
#define DATA_PROCESSING_LOGGER_H

const size_t TRANSMISSION_ARRAY_SIZE = 19;

extern uint8_t *transmissionDataArray[TRANSMISSION_ARRAY_SIZE];

extern uint8_t getTransmissionArrayData(void);
extern bool packTransmissionArray(void);

#endif
