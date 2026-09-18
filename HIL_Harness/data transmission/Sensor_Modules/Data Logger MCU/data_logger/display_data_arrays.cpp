#include "console_display_interface.h"
#include "error_codes_interface.h"
#include "system_snapshot.h"
#include "display_data_arrays.h"
#include <stdint.h>

typedef enum { 	INPUTS_SECTION_START 			= 0,
				COLOURS_SECTION_START 			= 4,
				JOYSTICK_SYSTEM_SECTION_START	= 9,
				SYSTEM_REPORTS_SECTION_START	= 12,
				SNAPSHOT_COUNTER_INDEX			= 17 } DisplayMatrixSections;

typedef enum {	JOYSTICK_SYSTEM_STATUS_INDEX = 0,
				JOYSTICK_ERROR_COUNTER_INDEX,
				RGB_SYSTEM_STATUS_INDEX,
				RED_STATUS_INDEX,
				GREEN_STATUS_INDEX,
				BLUE_STATUS_INDEX } ErrorMessageArraySections;
				
char consoleDisplayArray_Titles[DISPLAY_ARRAY_SIZE];
uint16_t consoleDisplayArray_Data[DISPLAY_ARRAY_SIZE];
char errorMessagesArray[ERROR_MESSAGES_ARRAY_SIZE];

bool displayArrayBuilt = 0;

static bool checkNewSectionStart(uint8_t index);
static bool buildErrorReportMessagesArray(void);

static bool checkNewSectionStart(uint8_t index) {
	
	bool newSection = (index == INPUTS_SECTION_START) ||  (index == COLOURS_SECTION_START) || (index == JOYSTICK_SYSTEM_SECTION_START) || (index == SYSTEM_REPORTS_SECTION_START);
	return newSection;
}

static bool buildErrorReportMessagesArray(void) {
	
	for (int i = 0; i < ERROR_MESSAGES_ARRAY_SIZE; ++i) {
		
		uint8_t snapshotIndex = i+ERROR_MESSAGES_ARRAY_SIZE;
		uint8_t systemErrorMessagesIndex = getSystemSnapshotData(snapshotIndex);
		
		switch(i) {
			case JOYSTICK_SYSTEM_STATUS_INDEX ... JOYSTICK_ERROR_COUNTER_INDEX: {
				errorMessagesArray[i] = joystickSystemStateMessages[systemErrorMessagesIndex];
			}
			case RGB_SYSTEM_STATUS_INDEX: {
				errorMessagesArray[i] = systemErrorMessages[systemErrorMessagesIndex];
			}
			case RED_STATUS_INDEX ... BLUE_STATUS_INDEX: {
				errorMessagesArray[i] = rangeErrorStatusMessages[systemErrorMessagesIndex];
			}
		}
	}
	return 1;
}

bool buildConsoleDisplayArrays(void) {
	
	displayArrayBuilt = buildErrorReportMessagesArray();
	
	if (!displayArrayBuilt) {
		return 0;
	}
	
	uint8_t sectionTitlesCounter = 0;
	
	for (int i = 0; i < SNAPSHOT_COUNTER_INDEX; ++i) {
		
		if (checkNewSectionStart(i)) {
			
			consoleDisplayArray_Titles[i] = sectionTitlesArray[sectionTitlesCounter];
			consoleDisplayArray_Data[i] = 0;
			++sectionTitlesCounter;
		} else {
			const uint8_t j = i-sectionTitlesCounter;
			consoleDisplayArray_Titles[i] = elementTitlesArray[j];
			consoleDisplayArray_Data[i] = getSystemSnapshotData(j);
		}
	}
	const uint8_t snapshotIndex = SNAPSHOT_COUNTER_INDEX - sectionTitlesCounter;
	consoleDisplayArray_Data[SNAPSHOT_COUNTER_INDEX] = getSystemSnapshotData(snapshotIndex);
	
	displayArrayBuilt = 1;
	return 1;
}
