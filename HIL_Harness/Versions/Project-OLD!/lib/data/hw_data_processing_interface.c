
// CONSTANTS
static const uint8_t ZERO_INPUT = 0;

// VARIABLES
static uint16_t potentiometerLevel = ZERO_INPUT;
static uint16_t joystickLevel = ZERO_INPUT;

static uint16_t readPotentiometer(void) {
	
	if (debounceHardware()) {
		
		potentiometerLevel = digitalRead(POT_PIN);	
	}
	return potentiometerLevel;
}

static uint16_t readJoystick(void) {
	
	if (debounceHardware()) {
		
		joystickLevel = digitalRead(JOYSTICK_PIN);
	}
	return joystickLevel;
}

bool newHardwareInputReceived(void) {
	
	uint16_t newPotInput = readPotentiometer();
	uint16_t newJoystickInput = readJoystick();
	
	bool newPotInput = newPotInput != potentiometerLevel;
	bool newJoystickInput = newJoystickInput != joystickLevel;
	
	bool newInputReceived = newPotInput || newJoystickInput;
	
	if (newInputReceived) {
		potentiometerLevel = newPotInput;
		joystickLevel = newJoystickInput;
	}
	
	return newInputReceived;
}

uint16_t getPotentiometerInput(void) {
	return potentiometerLevel;
}

uint16_t getJoystickInput(void) {
	return joystickLevel;
}
