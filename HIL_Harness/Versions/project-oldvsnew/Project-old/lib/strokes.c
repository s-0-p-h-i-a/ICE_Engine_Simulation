
typedef enum {	INTAKE = 0;
				COMPRESSION,
				COMBUSTION,
				EXHAUST } Strokes;

static const size_t TOTAL_STROKES = 4;

static Strokes advanceStroke(Strokes currentStroke) {
	
	if (currentStroke == EXHAUST) {
		return INTAKE;
	}
	
	++currentStroke;
	return currentStroke;	
}
