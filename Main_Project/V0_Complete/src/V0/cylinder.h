#include <stdint.h>

#ifndef CYLINDER_H
#define CYLINDER_H

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t cylinderElementStates[5];

extern int cylinderStateNow;

void updateCylinderState(void);
void updateCylinderElements(int state);

#ifdef __cplusplus
}
#endif

#endif
