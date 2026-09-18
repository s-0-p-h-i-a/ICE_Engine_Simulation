#include "system_snapshot.h"
#include "matrix_display.h"
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <arduino.h>

typedef enum {	POTENTIOMETER_COLUMN		= 0,
				WATER_LEVEL_SENSOR_COLUMN	= 4,
				RED_PAUSE_COLUMN			= 5 } DisplayDataColumns;
				
typedef enum {	CONVERT_RED_PAUSE_TO_ROW = 8,
				CONVERT_RGB_TO_ROW		= 32,
				CONVERT_ANALOG_TO_ROW	= 127 } ConversionValues;

static const size_t MATRIX_SIZE = 8;

static const uint8_t ROW_1_PIN = 2;
static const uint8_t ROW_2_PIN = 3;
static const uint8_t ROW_3_PIN = 4;
static const uint8_t ROW_4_PIN = 5;
static const uint8_t ROW_5_PIN = 6;
static const uint8_t ROW_6_PIN = 7;
static const uint8_t ROW_7_PIN = 8;
static const uint8_t ROW_8_PIN = 9;

static const uint8_t COLUMN_1_PIN = 10;
static const uint8_t COLUMN_2_PIN = 11;
static const uint8_t COLUMN_3_PIN = 12;
static const uint8_t COLUMN_4_PIN = 13;
static const int COLUMN_5_PIN = A2;
static const int COLUMN_6_PIN = A3;
static const int COLUMN_7_PIN = A4;
static const int COLUMN_8_PIN = A5;

static const uint8_t matrixRows[MATRIX_SIZE] = {ROW_1_PIN, ROW_2_PIN, ROW_3_PIN, ROW_4_PIN, ROW_5_PIN, ROW_6_PIN, ROW_7_PIN, ROW_8_PIN};
static const uint8_t matrixColumns[MATRIX_SIZE] = {COLUMN_1_PIN, COLUMN_2_PIN, COLUMN_3_PIN, COLUMN_4_PIN,
									COLUMN_5_PIN, COLUMN_6_PIN, COLUMN_7_PIN, COLUMN_8_PIN};

static uint8_t computeConversionRatio(int currentColumn);

static uint8_t computeConversionRatio(int currentColumn) {
	
	if (currentColumn <= WATER_LEVEL_SENSOR_COLUMN) {
		return CONVERT_ANALOG_TO_ROW;
	} else if (currentColumn == RED_PAUSE_COLUMN) {
		return CONVERT_RED_PAUSE_TO_ROW;
	} else {
		return CONVERT_RGB_TO_ROW;
	}
}

void initialiseMatrix(void) {
	
	for(int i=0; i < MATRIX_SIZE; ++i) {
		pinMode(matrixColumns[i], OUTPUT);
		pinMode(matrixRows[i], OUTPUT);
	}
	
	// set all columns to LOW so they can respond to rows voltage change
	for (int j=0; j < MATRIX_SIZE; ++j) {
		digitalWrite(matrixColumns[j], LOW);
	}
}
	
void displayDataMatrix(void) {
	
	for (int i=POTENTIOMETER_COLUMN; i < MATRIX_SIZE; ++i) {
		
		uint8_t level = round(getSystemSnapshotData(i) / computeConversionRatio(i));
		
		for (int k=0; k < level; ++k) {
			digitalWrite(matrixRows[k], HIGH);
		}
		
		if (level < MATRIX_SIZE) {
			for (int l=level; l < MATRIX_SIZE; ++l) {
				digitalWrite(matrixRows[l], LOW);
			}
		}
	}
}
	

