#include <stdint.h>
#include <stddef.h>

#ifndef DATA_INTERFACE_H
#define DATA_INTERFACE_H

#define UINT8_MAX_PLUS_1 256
#define ARRAY_CONST_VALUE 1

typedef enum {	POS0 = 0,
				POS1,
				POS2,
				POS3,
				POS4 } LocalArrayElements;

extern bool generateLocalArray(void);
extern bool getLocalArray(LocalArrayElements index);

#endif
