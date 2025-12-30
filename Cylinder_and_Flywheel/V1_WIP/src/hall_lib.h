#include <stdint.h>

#ifndef HALL_LIB_H
#define HALL_LIB_H

enum { HALL_PIN = 5 };

extern uint8_t hallON;
extern uint8_t secondPass;

void updateHallState(void);
unsigned long int getTimeWindow(void);
uint8_t readHallNow(void);

#endif
