#ifndef HALL_LIB_H
#define HALL_LIB_H

enum { HALL_PIN = 65 };

extern bool hallON;
extern bool secondPass;

extern long unsigned int currentSensorRead;
extern long unsigned int lastSensorRead;

void initHall(void);
void checkReadState(void);

#endif
