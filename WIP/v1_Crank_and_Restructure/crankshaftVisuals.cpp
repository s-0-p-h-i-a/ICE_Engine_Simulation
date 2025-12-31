
//	========= CONSTANTS =========
#define TOTAL_LOBES_AND_STATES 4
#define TDC 1
#define BDC 0
#define LOBE_1_STATES { 0, 1, 0, 1 }
#define LOBE_2_STATES { 1, 0, 1, 0 }
#define LOBE_3_STATES { 1, 0, 1, 0 }
#define LOBE_4_STATES { 0, 1, 0, 1 }

// LED Pins
enum { LOBE_1_PIN_TDC = 4, LOBE_2_PIN_TDC = 5, LOBE_3_PIN_TDC = 6, LOBE_4_PIN_TDC = 7 } PinsTDC;
enum { LOBE_1_PIN_BDC = 8, LOBE_2_PIN_BDC = 9, LOBE_3_PIN_BDC = 10, LOBE_4_PIN_BDC = 11 } PinsBDC;
// Array indexing
typedef enum { LOBE_1 = 0, LOBE_2 = 1, LOBE_3 = 2, LOBE_4 = 3 } Lobes;
// TDC LED pins array
static const unsigned int tdcPins[TOTAL_LOBES_AND_STATES] =
							{ LOBE_1_PIN_TDC, LOBE_2_PIN_TDC, LOBE_3_PIN_TDC, LOBE_4_PIN_TDC };
// BDC LED pins array							
static const unsigned int bdcPins[TOTAL_LOBES_AND_STATES] =
							{ LOBE_1_PIN_BDC, LOBE_2_PIN_BDC, LOBE_3_PIN_BDC, LOBE_4_PIN_BDC };

// Lobe states matrix
static const uint8_t lobeStatesMatrix[TOTAL_LOBES_AND_STATES][TOTAL_LOBES_AND_STATES] =
							{ LOBE_1_STATES, LOBE_2_STATES, LOBE_3_STATES, LOBE_4_STATES };


//	========= FUNCTIONS =========

void writeCrankshaftPins(unsigned int stroke) {
	
	for (int i = LOBE_1; i < TOTAL_LOBES; ++i) {
		
		digitalWrite(tdcPins[i], lobeStatesMatrix[i][stroke]);
		digitalWrite(tdcPins[i], !lobeStatesMatrix[i][stroke]);
	}
}
