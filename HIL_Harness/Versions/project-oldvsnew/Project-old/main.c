


int main(void) {
	
	setSystemStatus(MAIN_OK);
	
	if (!hardwareInit()) {
		setSystemStatus(HARDWARE_INIT_ERROR);
		return 0;
	}
	
	if (!engineSystemInit()) {
		setSystemStatus(SYSTEM_INIT_ERROR);
		return 0;
	}
	
	while (1) {
		if (!runSystemManager()) {
			setSystemStatus(SYSTEM_MANAGER_ERROR);
		}
	}
	return 0;
}
