#include "display_data_arrays.h"
#include "console_display.h"
#include <stdio.h>

typedef enum {	DISPLAY_ARRAY_START 	= 0,
				ERROR_REPORTS_START		= 10,
				SNAPSHOT_COUNTER_INDEX	= 17 } ConsoleDisplaySections;

void displayDataConsole(void) {
	
	buildConsoleDisplayArrays();
	
	printf("System snapshot number: %d /n", consoleDisplayArray_Data[SNAPSHOT_COUNTER_INDEX]);
	
	printf("/n");
	printf("I/O data values: /n");
	
	for (int i = DISPLAY_ARRAY_START; i < ERROR_REPORTS_START; ++i) {
		printf(" %c%d /n", consoleDisplayArray_Titles[i], consoleDisplayArray_Data[i]);
	}
	
	printf("/n");
	printf("System status reports: /n");
	
	for (int i = ERROR_REPORTS_START; i < SNAPSHOT_COUNTER_INDEX; ++i) {
		uint8_t j = i - ERROR_REPORTS_START;
		printf(" %c%d /n", consoleDisplayArray_Titles[i], consoleDisplayArray_Data[i], " = %c", errorMessagesArray[j]);
	}
}
	
	
