#ifndef CRANK_PISTON_H
#define CRANK_PISTON_H

#include <stdint.h>

extern bool crankshaftInit(void);
extern bool updateCrank(uint8_t currentEngineLoad);

#endif
