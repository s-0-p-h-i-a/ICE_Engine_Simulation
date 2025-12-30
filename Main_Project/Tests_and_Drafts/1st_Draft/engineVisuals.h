#ifndef ENGINEVISUALS_H
#define ENGINEVISUALS_H

enum {
  intakeValvePin    = 13,
  exhaustValvePin   = 2,
  fuelInjectorPin   = 12,
  sparkPlugPin      = 3,
  tdcPin1           = 11,
  tdcPin2           = 4,
  bdcPin1           = 10,
  bdcPin2           = 5
};

enum { TOTAL_PINS  = 8 };

void displayEngine();

void pistonWrite(bool pistonUP);

#endif
