 // ECU API => engine_api_ecu.h
 Strokes getCurrentStrokeIndex(void) {
	 Strokes currentStroke = getCurrentIndex();
	 return currentStroke;
 }
 
 
 
 
 // SYSTEM SNAPSHOT/REPORT API => engine_api_snapshot.h
bool intakeValveStateAPI(void) {
	bool intakeValveState = getIntakeValveState();
	return intakeValveState;
}

bool exhaustValveStateAPI(void) {
	bool exhaustValveState = getExhaustValveState();
	return intakeValveState;
}

bool fuelInjectorStateAPI(void) {
	bool fuelInjectorState = getFuelInjectorState();
	return fuelInjectorState;
}

bool sparkPlugStateAPI(void) {
	bool sparkPlugState = getSparkPlugState();
	return sparkPlugState;
}
