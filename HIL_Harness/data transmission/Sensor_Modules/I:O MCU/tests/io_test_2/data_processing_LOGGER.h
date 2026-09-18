#include <stddef.h>
#include <stdint.h>

#ifndef DATA_PROCESSING_LOGGER_H
#define DATA_PROCESSING_LOGGER_H

const size_t TRANSMISSION_ARRAY_SIZE = 7;

extern uint8_t transmissionArray[TRANSMISSION_ARRAY_SIZE];

extern bool buildTransmissionArray(void);
extern uint8_t getTransmissionArray(uint8_t arrayIndex);

#endif
