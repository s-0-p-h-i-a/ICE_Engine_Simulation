# ANALOG I/O + DATA LOGGING PROJECT

This project consists of a system with 2 MCUS and I/O peripherals connected via UART:
	- A control MCU that creates output based on user input and periodically sends data to the data logger MCU
	- Data logger MCU receives control system snapshots and processes it for visual display

---
# I/O MCU

This MCU combines analog input from a water level sensor, a joystick and a potentiometer to control an RGB LED.
Each input source controls the brightness of one RGB LED leg. The joystick can either control the red colour brightness or make it blink at full brightness.

It uses input and error handling modules to ensure proper execution.

I/O data is collected and processed for serial monitor display and transmission to a data logger MCU via UART.
Serial monitor visualisation is used to support hw/sw integration and debugging.

## Folder Structure & System Status:
- analog_io folder: user I/O MCU first draft code, compiled but not verified
- test folder: test scripts for progressive testing, fixing and reintegration of first draft code 

Current test in progress:
	- Test 2:
		- Transmitter MCU (I/O) sends a constant array[7] of 1's to the receiver MCU (data logger).
		- The receiver MCU then processes it into a local 5 element array.

- The first 3 elements of the transmitted array represent a bit-packed uint16 integer x = 256:
	- 1st element: x / 255 = 1
	- 2nd element: x % 255 = 1
	- 3rd element: boolean flag indicating whether uint16 x had to be bit-packed for uint8 representation
		-> if yes: = 1
		
The data logger output is then: { 256, 1, 1, 1, 1 }

This test excludes all other mechanisms in the I/O + data logging system project and simply focuses on validating UART data transmission.

---
# SYSTEM
## INPUT
- Joystick (analog 0-1023)
- Water level sensor (analog 0-1023)
- Potentiometer (analog 0-1023) 

## OUTPUT
- RGB LED (PWM 0-255)

Red = Joystick, Green =  Water level sensor, Blue = Potentiometer

---
## OUTPUT
### Joystick
- Neutral (511 ± hyst) = red OFF
- Up (±511 - 1023) -> red goes from 0 to 255
- Down (±511 - 0) -> red blinks progressively faster, delay 1000ms to 20 ms

### Water level sensor
- 0 = green 0 <-> 1023 = green 255

### Potentiometer
- 0 = blue 255 <-> 1023 = blue 0

---
## STRUCTURE
### Main program
- Hardware input interfaces read data from analog input sources
- Red light/joystick has intermediary mode selection module
- Control interfaces for each colour take read data, process it and use for individual RGB leg actuation
- Main RGB module calls individual colour control functions
- Main calls RGB control function

### Input handling and error tracking
- Independent joystick input error handling system
- Range handling module prevents values outside of valid ranges
- Error reports module generates error report array every loop using error codes

### Data transmission
- Analog uint16 values from input sources are bit-packed into a 3 element uint8 array:
	- input value / 255 (integer division)
	- input value % 255
	- flag: true if input value > 255
- Data procession/transmission interfaces create data pack array, send to data logger MCU via UART
- Transmission data array:
	- 3x 3 element bit-packed data arrays: analog input values
	- 4x colour control values: brightness and red colour blink cycle
	- 2x joystick system status: error status code, error counter
	- 4x system data: whole system status code, 3x colour control status codes

### Helper libraries
- Main raw data handling interface
- Data processing for arduino IDE serial plotter display
- Data processing for data logger MCU

!! NOT UPDATED !!
### Dependencies
- Hardware:
	- Input reading only modules, do not include anything
- Red light:
	- Mode selection module:
		- Includes range handling module
		- Enum for input modes (used by control interface)
	- Control interface module:
		- Includes:
			- Joystick input reading module
			- Joystick input error handling module
			- Mode selection module
			- Range handling module
		- Uses input modes enum from mode selection
- Other lights:
	- Control interface modules:
		- Include:
			- Associated hardware input modules
			- Range handling module
- Central RGB control:
	- Includes:
		- Individual colour control modules
	- Calls their respective actuation functions
	- Macros for pin numbers
- Main:
	- Includes RGB control module, calls main RGB control function, uses pin number macros from RGB module for setup
	#include "rgb_led.h"
#include "plotter_display.h"
#include "logger_transmission.h"
#include "error_reports.h"
- Error reports module:
	- Includes include "error_handling_JOYSTICK.h"
#include "control_RED.h"
#include "control_GREEN.h"
#include "control_BLUE.h"
- Visualisation:
	- Raw data interface: includes hardware and colour control modules
	- Data processing (plotter): includes raw data interface
	- Data processing (logger): includes raw data interface
	- Plotter module: includes data processing interface (plotter)
	- Logger module: includes data processing interface (logger)

---
!! NOT UPDATED !!	
## BEHAVIOUR
### Intended execution path
A loop cycle starts:
- (main.ino) -> main calls RGB control function in void loop
	- (rgb_led.cpp) -> central RGB control function calls control function from a colour
		- (control_COLOUR.cpp) -> a colour control function calls hardware read function*
			- (hardware_input.cpp) -> read function takes hardware input, sends to colour control function, finishes execution
		- (control_COLOUR.cpp) -> colour control function uses hardware input data to control colour pin, finishes execution
	- (rgb_led.cpp)	-> RGB function calls the two other colour control functions finishes execution
- (main.ino) -> void loop finishes execution

*red control function calls both hardware input function and signal processing function

!! NOT UPDATED !!
### Safety
- Range handling header:
	- limitToAnalogMax for hardware input values, max 1023
	- limitToRGBMAX for translated input values, max 255
	- clampToRGBRange to translate input values from analog range to RGB range
- Enums/states for error handling
- Red light control module as sandbox to brainstorm error handling

!! NOT UPDATED !!
### Visualisation & data logging
- Raw data interface takes I/O values
- Data processing (plotter): shifts/amplifies data for clear plotter visuals
- Data processing (logger): provides data for transmission via UART

---
# TO-DO:
I/O MCU:
- Update functions index, README and dev notes
- Compile checks/reviews
- HW/SW integration: one MCU + serial plotter display
- Update functions index
	
DATA LOGGER MCU:
- Start documentation
- Compile check DONE
- Functions index

FULL SYSTEM:
- Full system integration:
	- Run planned tests
	- Validate behaviour
	- Progressively build PoC
- Communication integration
- Make structure/execution diagrams
- Make state diagrams:
	- FSMs: colour control, error handling
	- Block+register: overall logic, input handling
- Make schematics
- Create demo content
- Consolidate documentation + demos
