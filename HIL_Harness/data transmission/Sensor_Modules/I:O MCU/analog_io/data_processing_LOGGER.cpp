#include "data_interface.h"
#include "error_reports.h"
#include "data_processing_LOGGER.h"
#include <stddef.h>
#include <stdint.h>

// CONSTANTS
typedef enum { 	BATCH_1_END 	= 2, // potentiometer
				BATCH_2_START 	= 3, // joystick
				BATCH_2_END 	= 5,
				BATCH_3_START	= 6, // water level sensor
				BATCH_3_END		= 8,
				BATCH_4_START	= 9, // red brightness, red pause, green brightness, blue brightness
				BATCH_4_END		= 12,
				BATCH_5_START	= 13, // joystick system status, joystick system error counter
				BATCH_5_END		= 18 } TransmissionArraySections;
				
const size_t BITPACKING_ARRAY_SIZE = 3;
const size_t COLOUR_MODULES_ARRAY_SIZE = 4;
const size_t SYSTEM_REPORTS_ARRAY_SIZE = 5;
				
static const uint8_t MAX_VALUE = 255;

// DATA ARRAYS
static uint8_t *potentiometerInput_DataPack[BITPACKING_ARRAY_SIZE] = { NULL };

static uint8_t *joystickInput_DataPack[BITPACKING_ARRAY_SIZE] = { NULL };

static uint8_t *waterLevelSensorInput_DataPack[BITPACKING_ARRAY_SIZE] = { NULL };

static uint8_t *colourModulesDataArray[COLOUR_MODULES_ARRAY_SIZE] = { NULL };

static uint8_t *systemReportsDataArray[SYSTEM_REPORTS_ARRAY_SIZE] = { NULL };

uint8_t *transmissionDataArray[TRANSMISSION_ARRAY_SIZE] = { NULL };

// FORWARD DECLARATIONS
static uint8_t bitPacking16to8_High(uint16_t valueToPack);
static uint8_t bitPacking16to8_Low(uint16_t valueToPack);
static uint8_t bitPacking16to8_Flag(uint16_t valueToPack);

static bool buildIOValuesDataArray(void);
static bool buildSystemReportDataArray(void);

// FUNCTIONS
//// BIT PACKING
////// HIGH
static uint8_t bitPacking16to8_High(uint16_t valueToPack) {
	
	if (valueToPack <= MAX_VALUE) {
		return valueToPack;
	} else {
		return valueToPack / MAX_VALUE;
	}
}
////// LOW
static uint8_t bitPacking16to8_Low(uint16_t valueToPack) {
	return valueToPack % MAX_VALUE;	
}
////// FLAG
static uint8_t bitPacking16to8_Flag(uint16_t valueToPack) {
	
	bool bitPackingFlag = bitPacking16to8_Low(valueToPack) > 0;
	
	if (bitPackingFlag) {
		return 1;
	} else if (!bitPackingFlag) {
		return 0;
	}
}

static bool checkIfBitPackingSucceeded(uint8_t high, uint8_t low, uint8_t flag) {
	
	bool bitPackingFailed = (&high == NULL) || (&low == NULL) || (&flag == NULL);
	
	if (bitPackingFailed) {
		return 0;
	}
	return 1;
}
	
// ARRAY
////// I/O VALUES
static bool buildIOValuesDataArray(void) {
	
	uint8_t potentiometerInput_DataPack[BITPACKING_ARRAY_SIZE] 	 = { bitPacking16to8_High(potentiometer_Input),																	bitPacking16to8_Low(potentiometer_Input),									 							 bitPacking16to8_Flag(potentiometer_Input) };
	for (int i = 0; i < BITPACKING_ARRAY_SIZE; ++i) {
		if (potentiometerInput_DataPack[i] == NULL) {
			return 0;
		}
	}
	
	uint8_t joystickInput_DataPack[BITPACKING_ARRAY_SIZE] 	  	 = { bitPacking16to8_High(joystick_Input),															   	   bitPacking16to8_Low(joystick_Input),									 							   bitPacking16to8_Flag(joystick_Input) };
	
	for (int i = 0; i < BITPACKING_ARRAY_SIZE; ++i) {
		if (potentiometerInput_DataPack[i] == NULL) {
			return 0;
		}
	}
	
	uint8_t waterLevelSensorInput_DataPack[BITPACKING_ARRAY_SIZE] = { bitPacking16to8_High(waterLevelSensor_Input),																 	 bitPacking16to8_Low(waterLevelSensor_Input),									 							  bitPacking16to8_Flag(waterLevelSensor_Input) };
	
	for (int i = 0; i < BITPACKING_ARRAY_SIZE; ++i) {
		if (potentiometerInput_DataPack[i] == NULL) {
			return 0;
		}
	}
	
	uint8_t colourModulesDataArray[COLOUR_MODULES_ARRAY_SIZE] = { redBrightnessLevel_Data, redPauseTime_Data,
															greenBrightnessLevel_Data, blueBrightnessLevel_Data };
															
	for (int i = 0; i < COLOUR_MODULES_ARRAY_SIZE; ++i) {
		if (colourModulesDataArray[i] == NULL) {
			return 0;
		}
	}
	return 1;
}
															
static bool buildSystemReportDataArray(void) {
	
	for (int i = JOYSTICK_STATE_POSITION; i <= BLUE_POSITION; ++i) {
		systemReportsDataArray[i] = getErrorReportArrayElements(i);
		
		if (systemReportsDataArray[i] == NULL) {
			return 0;
		}
	}
	return 1;
}

uint8_t getTransmissionArrayData(uint8_t index) {
	return transmissionDataArray[index];
}

bool packTransmissionArray(void) {
	
	buildIOValuesDataArray();
	
	if (!buildIOValuesDataArray()) {
		return 0;
	}
	
	buildSystemReportDataArray();
	
	if (!buildSystemReportDataArray()) {
		return 0;
	}
	
	for (int i = 0; i < TRANSMISSION_ARRAY_SIZE; ++i) {
		
		switch(i) {
			case 0 ... (BATCH_1_END): {
				transmissionDataArray[i] = potentiometerInput_DataPack[i];
			} break;
			
			case BATCH_2_START ... BATCH_2_END: {
				uint8_t j = i - BATCH_2_START;
				transmissionDataArray[i] = joystickInput_DataPack[j];
			} break;
			
			case BATCH_3_START ... BATCH_3_END: {
				uint8_t j = i - BATCH_3_START;
				transmissionDataArray[i] = waterLevelSensorInput_DataPack[j];
			} break;
			
			case BATCH_4_START ... BATCH_4_END: {
				uint8_t j = i - BATCH_4_START;
				transmissionDataArray[i] = colourModulesDataArray[j];
			} break;
			
			case BATCH_5_START ... BATCH_5_END: {
				uint8_t j = i - BATCH_5_START;
				transmissionDataArray[i] = systemReportsDataArray[j];
			} break;

			default: {
				return 0;
			}
		}
		if (transmissionDataArray[i] == NULL) {
			return 0;
		}
	}
	return 1;
}

