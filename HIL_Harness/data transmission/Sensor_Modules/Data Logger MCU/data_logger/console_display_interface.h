#include <stdint.h>
#include <stddef.h>

#ifndef CONSOLE_DISPLAY_INTERFACE_H
#define CONSOLE_DISPLAY_INTERFACE_H

#define SECTION_1_TITLE "INPUTS:"
#define SECTION_2_TITLE "COLOURS:"
#define SECTION_3_TITLE "JOYSTICK SYSTEM:"
#define SECTION_4_TITLE "RGB SYSTEM:"

#define ELEMENT1 "Potentiometer: "
#define ELEMENT2 "Joystick: "
#define ELEMENT3 "Water level sensor: "
#define ELEMENT4 "Red brightness: "
#define ELEMENT5 "Red blink pause: "
#define ELEMENT6 "Green brightness: "
#define ELEMENT7 "Blue brightness: "
#define ELEMENT8 "Joystick system status: code "
#define ELEMENT9 "Joystick error counter: code "
#define ELEMENT10 "RGB system status: code "
#define ELEMENT11 "Red status: code "
#define ELEMENT12 "Green status: code "
#define ELEMENT13 "Blue status: code "

typedef enum { 	INPUTS = 0,
				COLOURS,
				JOYSTICK_SYSTEM,
				RGB_SYSTEM } SectionTitles;
				
static const size_t SECTION_TITLES_ARRAY_SIZE = 4;
static const size_t LOCAL_ARRAY_SIZE = 13;
				
extern const char *sectionTitlesArray[SECTION_TITLES_ARRAY_SIZE] = {SECTION_1_TITLE, SECTION_2_TITLE, SECTION_3_TITLE, SECTION_4_TITLE};

extern const char *elementTitlesArray[LOCAL_ARRAY_SIZE] = {ELEMENT1, ELEMENT2, ELEMENT3, ELEMENT4, ELEMENT5, ELEMENT6, ELEMENT7, ELEMENT9, ELEMENT10, ELEMENT11, ELEMENT12, ELEMENT13};
#endif
