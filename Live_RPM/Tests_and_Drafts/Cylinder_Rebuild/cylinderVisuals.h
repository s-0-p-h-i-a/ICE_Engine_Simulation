#ifndef CYLINDERVISUALS_H
#define CYLINDERVISUALS_H

enum { 
  exhaustValvePin   = 2,
  sparkPlugPin      = 3,
  tdcPin2           = 4,
  bdcPin2           = 5,
  bdcPin1           = 10,
  tdcPin1           = 11,
  fuelInjectorPin   = 12,
  intakeValvePin    = 13
};

void pistonWrite(bool pistonUP);
void writePins(void);
void clearDisplay(void);
void displayCylinder(int pause);

#endif
