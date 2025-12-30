#include <stdbool.h>

#ifndef ENGINE_H
#define ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
  INTAKE      = 0,
  COMPRESSION = 1,
  COMBUSTION  = 2,
  EXHAUST     = 3
};

enum {
  TOTAL_CYCLE       = 4,
  CYLINDER_ELEMENTS = 5
};

enum {
  INTAKE_VALVE    = 0,
  EXHAUST_VALVE   = 1,
  FUEL_INJECTOR   = 2,
  SPARK_PLUG      = 3,
  PISTON          = 4
}; 

enum {
	BDC = 0,
	TDC = 1
};

enum { IDLE_INTERVAL = 10000 };

extern bool cylinder[CYLINDER_ELEMENTS];

extern bool isIdle;
extern bool engineON;
extern int speed;

void updateStroke(void);

void runEngine(void);

void startEngine(void);

#ifdef __cplusplus
}
#endif

#endif
