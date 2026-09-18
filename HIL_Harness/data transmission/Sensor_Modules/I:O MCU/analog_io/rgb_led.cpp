#include "control_RED.h"
#include "control_GREEN.h"
#include "control_BLUE.h"
#include "rgb_led.h"

void controlRGB(void) {
	controlRedLight(RED_PIN);
	controlGreenLight(GREEN_PIN);
	controlBlueLight(BLUE_PIN);
}
