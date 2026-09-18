#ifndef ERROR_REPORTS_H
#define ERROR_REPORTS_H

typedef enum {	JOYSTICK_STATE_POSITION = 0,
				JOYSTICK_ERROR_COUNTER_POSITION,
				SYSTEM_STATE_POSITION,
				RED_POSITION,
				GREEN_POSITION,
				BLUE_POSITION } ReportArrayPositions;

typedef enum {	NO_ARRAY_BUILD_ERRORS = 0,
				JOYSTICK_STATUS_ASSIGNMENT,
				COLOUR_STATUS_ASSIGNMENT,
				SYSTEM_STATUS_ASSIGNMENT,
				ARRAY_GENERATION } ReportArrayBuildProcessPhases;

void printErrorReportArrayBuildError(void);
extern uint8_t getErrorReportArrayElements(uint8_t index);
extern bool generateErrorReportArray(void);

#endif
