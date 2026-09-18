# ANALOG I/O + DATA LOGGING PROJECT

This project consists of a system with 2 MCUS and I/O peripherals connected via UART:

-  An I/O control MCU that creates output based on user input and periodically sends data to the data logger MCU
- Data logger MCU receives control system snapshots and processes it for visual display

Project navigation:
- Current/latest work for each MCU is IO_MCU and Data_Logger_MCU folders
- Previous drafts/iterations in Previous_Drafts folder

---
## Project Status
- First draft code complete and compile reviewed
- Rebuilding working code through test plan and test scripts
- Current test in progress:
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

## MCUs
1. I/O control MCU:
	- Input: potentiometer, joystick, water level sensor
	- Output: RGB LED legs
	- Each of the 3 input peripherals controls one leg of the RGB LED
	- The combined output of each LED leg creates variations in displayed colour
	- Packs system data and transmits to data logger MCU
			
2. Data logger MCU:
	- Receives control MCU data and unpacks it
	- Processes data for 2 different outputs
	- 2 outputs:
		- Data is processed into system snapshots that are displayed on the console
		- Data is processed into actuator data to control an 8x8 LED Matrix

## DATA	
- Transmitted data:
	- I/O data:
			- Analog input values
			- RGB values for each colour and red LED leg blink cycle duration
	- Status reports:
		- Joystick system status:
			- Joystick input processing status code
			- Joystick input processing error counter
		- System wide status: 
			- Overall system status code
			- Individual status codes for each RGB LED leg

## REQUIREMENTS

### Implementation goals
- Limit data and function execution scopes+lifecycles to only when relevant/necessary
- Direct paths from input to actuation
- Limit dependencies
- Clear roles for each module
- Clear and readable semantics

### Intended system type
- The design choices are influenced by:
	- Performance-oriented control firmware (eg motorsports ECU)
	- Portability for future fault injection integration
- In practice:
	- As little concurrent processes as possible
	- System response as immediate as possible
	- Integrate error + undefined behaviour prevention/handling

---
# LEARNING GOALS

The goal of this project is to create a comprehensive sandbox to practice various SWE concepts + start learning hardware communications.

## Learning topics
- System structure design:
	- Architecture
	- Dependencies
	- Encapsulation
	- Modularisation
	- Data flow
	- Execution paths
		
- Safety integration:
	- Preventing undefined behaviour and invalid inputs
	- Implementing error handling systems (error codes, error messages)
	- Defining response behaviour when errors occur
	- Error traceability
		
- Documentation:
	- Writing and processing structured notes during development, instead of taking raw notes and processing later
	- Notes structure:
		- Progressively build a README: describe system as it is developed for more accurate explanations
		- Progressively build a dev log: track progress and explain build process
	- Mapping out program structure, behaviour and dependencies in writing before development
	- In-code comments
	- Functions index
		
- Communications:
	- First steps in learning to implement systems communication via UART
	- Connecting I/O system with data logger MCU to display system snapshots in console
	
- Fault injection preparation:
	- Creating mental model and implementing system security measures with future fault injection in mind

## New syntax/topics learned:
- Pointers and dereferencing to check whether an array has been populated (ptr != NULL)
- Array of functions for bit-packing on TX side
- Arrays as function arguments for bit-unpacking on RX side

---
# PROCESS
## Planning
- Define:
	- Desired behaviour
	- Program structure
	- Dependencies
- Map out intended execution paths

## Initial development
- Build base: input, colour control and LED control modules, main .ino file
- Integrate input handling/safety measures
- Write data interface modules
- Write data logger MCU code

## SW Review
- Compile checks:
	- Start with only one input + colour control combination, LED, and safety modules (green or blue for simplicity)
	- Add the 2 other input + colour control systems
	- Add data interfaces
	- Check data logger MCU

## HW/SW integration
### Communication:
- Run test plan to progressively build toward validated communication between TX and RX without user input

###Control response:
- Using plotter display:
	- Start with only one input + colour control
	- Add the 2 other input + colour combos
	- Exclude data logger interface
- Once plotter display is verified & validated, move on to communication with data logger:
	- Level 1: data interface sends one constant variable to logger, test for different values
	- Level 2: integrate one hardware input source, then its associated colour control values
	- Level 3: integrate rest of system

---
# TO-DO
## FULL SYSTEM:
- Communication integration

## I/O MCU: 
- HW/SW integration: one MCU + serial plotter display
- Update README, dev notes/log, functions index

## DATA LOGGER MCU:
- Start documentation
- Functions index

## DOCUMENTATION
- Make structure/execution diagrams
- Make state diagrams:
	- FSMs: colour control, error handling
	- Block+register: overall logic, input handling
- Make schematics
- Create demo content
- Consolidate documentation + demos
