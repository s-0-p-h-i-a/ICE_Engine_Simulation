#include <stdbool.h>

#ifndef CYLINDER_H
#define CYLINDER_H

#ifdef __cplusplus
extern "C" {
#endif

extern bool cylinderElementStates[5];
extern bool switchCylinderState;

void updateCylinderState(void);
void updateCylinder(void);
void runCylinder(void);

#ifdef __cplusplus
}
#endif

#endif
