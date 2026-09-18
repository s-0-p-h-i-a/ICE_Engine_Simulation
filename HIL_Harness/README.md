# ================ ENGINE DATA SIMULATION AND LOGGING SYSTEM - TX SIDE ================

# ================ MAIN PROJECT ================
This project is an ICE data simulation and logging system with 2 MCUs:

- One MCU (TX side) responds to user input, produces engine behaviour data, and transmits a system snapshot to the RX side
- The other MCU (RX side) receives the data and displays it on the Arduino serial plotter.

It is intended as a baseline to be later developed into a HIL-like fault injection system.

TX side:
- STM32F4 Nucleo board
- Breadboard
- Joystick
- Potentiometer
- LED

RX side:
- Arduino Uno
- Arduino IDE serial terminal and plotter

The learning goals of this project are to practice:
- Deterministic systems control
- Data processing and transmission
- Explicit implementation of FSMs
- Firmware design and architecture
- Simulation and abstraction of physical processes

# ================ TX SIDE: ENGINE DATA SIMULATION AND TRANSMISSION ================
## ======== OVERVIEW ========
This MCU takes three types of user input:
- Joystick button: engine on/off
- Joystick up: braking (analog ~500 to 1023 converted to -1 -> -10)
- Potentiometer: throttle (analog 0 to 1023 converted to 1 -> 10)

It simulates the following engine data based on user input:
- Engine load (1-100%)
- Crankshaft angle (°)
- Piston position (TDC, BDC, transition state)
- Flywheel angular speed (°/100ms)
- Camshaft angle (°)
- Intake and exhaust valves open/closed
- Fuel injector and spark plug on/off

The data transmitted is bit-packed into 2 32-bit data packets:
- Engine behaviour data (see above)
- Engine system error status codes

## ======== PROJECT STATUS ========
### 26/04
Preparing for first compile check, leaving data processing and transmission for later
- First draft for all .c files complete
- Documentation being updated

### Next steps:
- Finish documentation update
- Define includes structure
- Write header files
- First compile check
- Create simulation data header using datasheet

## ======== STRUCTURE ========
The file structure matches the functional structure: each module and layer has a specific set of responsibilities and receives data from/sends it to only the other relevant modules.

- Simple main.c: initialises system and runs system manager
- Src/include:
	- System manager
	- Event handler
	- Engine manager
	- Crankshaft and piston module
	- Camshaft module
	- Cylinder head module:
		- Valves module
		- Fuel and spark module
	- Engine API for data processing + transmission
	- Data transmission (to be added)
- Libraries:
	- Hardware interface
	- Angles (angle value constants, angle incrementation and wrapping helper functions)
	- Strokes (stroke cycle phases enum, advance stroke helper function)
	- Data processing

## ======== BEHAVIOUR ========
### START:
The system manager triggers the initialisation of all modules under it. There is a direct chain of init function calls from the system manager down to all the other module layers. All error statuses are set to 0 = no error.

The event handler polls for new throttle or brake input every 500ms.

The engine is off at start. The user toggles it on/off using the joystick button. All engine components are initialised to their states at the beginning (TDC) of the intake phase.

If the engine stays OFF: nothing is altered, engine component update functions do not run, data transmitted at a constant time interval.

### Engine ON:
If the user turns the engine on, it goes into IDLE mode and stays there until any further input is received.

Engine-wide update calls:
- Engine manager updates engine system
- Engine manager calls crankshaft update
	- Crank update updates crank system, calls camshaft update
		- Cam update updates cam system, calls valves update (if relevant open/close angle has been passed)
			- Valve states are updated
- Engine manager calls fuel+spark update
	- Injector and plug states are updated

### Input:
User gives throttle or brake input using the hardware peripherals.

Input is processed by the engine manager:
- Net input (positive or negative) is calculated as input = throttle input - brake input
- New engine load is calculated by adding input to load + clamped it to 20% - 100% range

Engine manager sends updated load value to crankshaft module:
- Crankshaft module translates engine load % to angle increment value in °/100ms

### No input:
If now new input is received, the engine stays at the same angular speed for a fixed duration of time, then decelerated down to idle speed. 
	
### Data transmission:
- Data transmission module orders new updated data packets using helper function library
- Data is transmitted to RX MCU

## ======== EXECUTION PATHS ========
### Initialisation
- Main.c calls system init function
	- System init calls engine init
		- Engine init calls crankshaft+piston init
			- Crankshaft+piston init calls camshaft init
				- Camshaft init calls valves init
					- Valves module initialises
		- Engine init calls fuel+spark init
			- Fuel and spark module initialises
		- Engine manager init done
	- System init done
- Back to main

### If the engine is turned ON:
If user turns the engine off at any point:
* All components reset to their initialisation state, all updates from engine manager down are skipped until engine is back on
* On/off overrides all other input

- Main.c calls the system manager, loop starts:
	- System manager.c calls event handler to check if new input happened this loop
	- System manager calls engine manager
	
If no user input (throttle or brake) happens this loop:
		- Engine manager.c calls crankshaft update
			- Crankshaft.c calls camshaft update
				- Camshaft.c calls valves update 
					- Valves.c updates valve states
			- Fuel and spark.c updates injector and spark plug states

If new input is detected:
		- Engine manager calls throttle and brake module to get new input value
		- Engine manager computes the new engine load
		- Engine manager calls the crank module update and feeds it the new load value
			- Crankshaft module converts load value to angle increment values (°/100ms)
			- Crank module updates crank angle and calls camshaft update
				- Camshaft module updates its angle using 2:1 gear ratio
				- Camshaft module calls valves update when a valve even threshold angle is passed
					- Valves module checks stroke state and performs the relevant valve state update
		- Engine manager calls fuel and spark update
			- Fuel and spark module updates injector and ignition state using updated stroke phase and cam angle info from the camshaft module

Data transmission:			
	- System manager calls the data transmission module:
			- Data transmission module checks if interval time has passed
			- If so, orders new data packets from the data processing module
				- Data processing calls engine data API module
					- Engine data API provides current data
				- Data processing creates 1st data packet
				- Data processing calls error statuses module
					- Error statuses module provides data
				- Data processing creates 2nd data packet
			- Data transmission sends 1st packet to RX MCU
			- Data transmission sends 2nd packet to RX MCU

Finish loop:
	- System manager finishes running
- Back to main.c -> next loop starts

## ======== ERROR HANDLING ========
### Error code enums:
- All NO_ERROR / SYSTEM_OK codes set to 0
- System wide, localised or combined errors can be found by adding or ORing error codes
	
### Error detection chains:
- All init and update functions are bool and return 0 if an error happens inside
- As shown in Execution Paths section: all init and update functions form function call chains
- All errors flow upwards to the system manager and trigger a main loop exit
- The error code tracks when the last error point happened

#### Example: Valves update fails
* Error in valves module
	- Detected by cam module when updateValves returns 0
	- -> Local cam module error code set to VALVES_UPDATE_ERROR
		- UpdateCamshaft returns 0 when called by updateCrankshaft
		- -> Local crank module error code = CAM_UPDATE_ERROR
			- UpdateCrankshaft returns 0 when called by engine manager
			- -> Local engine manager code set to MANAGE_ENGINE_STATE_ERROR
				- EngineManager returns 0 when called by system manager
				- -> Local system manager error code set to ENGINE_MANAGER_ERROR
					- System manager returns 0 when called inside while loop in main
					- -> While loop breaks, program execution ends 
	
## ======== STATE MACHINES ======== 
This project uses state enums and switch branches to implement state machines to control individual modules. The main FSM is in the engine manager. Lower modules use the same structure locally.

### ==== Engine system ====
* The engine has 4 states:
				ENGINE_IDLE,
				ENGINE_INPUT,
				ENGINE_COAST,
				ENGINE_AUTO_DECEL,
				ENGINE_OFF
	- Z0 = Idle
	- Z1 = Input
	- Z2 = Coast
	- Z3 = Auto deceleration after coast interval over
	- Z4 = Off

* UpdateEngineState updates the State at the start of each loop and manageCurrentEngineState controls the engine based on which state it is in.
	
#### == State transitions == 
##### Z0: Idle
- Z0 <-> Z0 (idle <-> idle), Z0 -> Z1 (idle -> input), Z0 -> Z4 (idle -> off)
- Z3 -> Z0 (auto decel -> idle), Z4 -> Z0 (off -> idle)

##### Z1: Input
- Z1 <-> Z1 (input <-> input), Z1 -> Z2 (input -> coast), Z1 -> Z4 (input -> off)
- Z0 -> Z1 (idle -> input), Z2 -> Z1 (coast -> input), Z3 -> Z1 (auto decel -> input)

##### Z2: Coast
- Z2 <-> Z2 (coast <-> coast), Z2 -> Z1 (coast -> input), Z2 -> Z3 (coast -> decel),  Z2 -> Z4 (coast -> off)
- Z1 -> Z2 (input -> coast)

#### Z3: Auto deceleration
- Z3 <-> Z3 (auto decel <-> auto decel), Z3 -> Z0 (auto decel -> idle), Z3 -> Z1 (auto decel -> input), Z3 -> Z4 (auto decel -> off)
- Z2 -> Z3 (coast -> auto decel)

#### Z4: Off
- Z4 <-> Z4 (off <-> off), Z4 -> Z0 (off -> idle)
- Z0 -> Z4, Z1 -> Z4, Z2 -> Z4, Z3 -> Z4 (engine can be turned off at any state)

#### State transitions:
UpdateEngineState runs at the beginning of the loop, using the data from the previous loop.
 -> Current state reflects the entire set of conditions/state/data from the entire previous loop.

#### Example: Engine system reaches the end of coast duration and no input is detected
##### Loop x: Coast duration reached during this loop
- Start of loop:
	- Before any instructions are executed:
		- currentState == Z2
	- updateEngineState() called as first instruction
		- coastingTimerElapsed() returns false
		- -> current state is now still Z2 (coasting)
- Z2 state will persist for rest of loop
- End of loop:
	- currentState == Z2

##### Loop x+1: System detects that coast time is over
- Start of loop:
	- Before any instructions are executed:
		- currentState == Z2
	- updateEngineState() called as first instruction
		- coastingTimerElapsed() returns true
		- -> current state is now Z3 (deceleration)
- Z3 state will persist for rest of loop
- No input this loop
- End of loop:
	- currentState == Z3
	
##### Loop x+2: First loop in auto decel state
- Start of loop:
	- Before any instructions are executed:
		- currentState == Z3
	- updateEngineState() called as first instruction
		- engineLoad == IDLE_LOAD returns false (hasn't reached idle load yet)
		- -> current state is still Z3
- Z3 state will persist for rest of loop
- No input this loop
- End of loop:
	- currentState == Z3 still
	
##### Repeat until engineLoad == IDLE_LOAD
If engineLoad == IDLE_LOAD becomes true during loop x+n, system will enter loop x+n+1 in Z3 and change to Z0 when updateEngineState() gets called.

## ======== BUILD PROCESS SO FAR ========
### Plan:
- Create folders and files to sketch program structure
- Write plans, goals, limitations/abstractions, intended system stucture, behaviour and execution paths
- Draw state machine graphs and write down states + state paths

### Draft:
- Start sketching basic code structure in main and manager modules
- Work from the outside in implementing broad then localised logic
- Move a layer or more up when localised logic points to improvements/changes needed in the layers above
- Use dev log to journal changes and decisions made

### Consolidate:
- Complete first draft of main, system manager, event handler, and engine modules
- First review: refactor where needed to improve/simplify logic
- Update README, write datasheet, finish abstractions/known issues/to do document, create dev log summary
