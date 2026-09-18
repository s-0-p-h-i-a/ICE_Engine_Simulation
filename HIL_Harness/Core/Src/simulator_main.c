/**
# ENGINE DATA SIMULATOR - TX SIDE

Behaviour:
- Creates ICE engine simulation data (throttle input, angular speed, etc)
- Packs data into 32-bit transmission packets
- Sends data to data logger RX

Simulator_main.c:
- Simple system 'driver'
- Separate from real main.c to encapsulate simulator code away from HAL code
- Prevents system manager execution if initialisation fails

**/

#include "simulator_main.h"
#include "system_manager_main.h"

bool simulator_main(void) {
	
	simulatorStatus = SIMULATOR_OK;
	
	if (!systemInit()) {
		simulatorStatus = SIMULATOR_INIT_ERROR;
		return 0;
	}
	
	while (1) {
		if (!runSystemManager()) {
			simulatorStatus = SIMULATOR_SYSMANAGER_ERROR;
			return 0;
		}
	}
	return 1;
}
