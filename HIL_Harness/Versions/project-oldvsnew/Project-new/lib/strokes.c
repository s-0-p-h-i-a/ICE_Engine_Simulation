/**
Strokes library

- 4-stroke cycle phase names
- Advance/wrap stroke phase function

**/

#include "./include/strokes.h"

// DEFINITIONS
typedef enum {	INTAKE = 0;
				COMPRESSION,
				COMBUSTION,
				EXHAUST } Strokes;

// CONSTANTS
static const size_t TOTAL_STROKES = 4;

// FUNCTIONS
Strokes advanceStroke(Strokes stroke) {
	
	Strokes newStroke = stroke;
	
	if (newStroke == EXHAUST) {
		return INTAKE;
	}
	
	++newStroke;
	return newStroke;	
}
