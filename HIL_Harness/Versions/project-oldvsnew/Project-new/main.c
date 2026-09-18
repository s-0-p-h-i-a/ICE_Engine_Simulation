/**
# ENGINE DATA SIMULATOR - TX SIDE

Behaviour:
- Creates ICE engine simulation data (throttle input, RPM, etc)
- Packs data into 32-bit transmission packets
- Sends data to data logger RX

Main.c:
- Simple system 'driver'
- Prevents system manager execution if initialisation fails

**/

#include "system_manager_main.h"

int main(void) {
	
	systemInit();
	
	while (1) {
		if (!runSystemManager()) {
			break;
		}
	}
	
	return 0;
}
