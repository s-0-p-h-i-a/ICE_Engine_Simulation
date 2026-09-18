#include "water_level_sensor.h"
#include "arduino.h"
#include <stdint.h>

uint16_t readWaterLevelSensorInput(uint8_t waterLevelSensorPin) {
	
	return analogRead(waterLevelSensorPin);
}
