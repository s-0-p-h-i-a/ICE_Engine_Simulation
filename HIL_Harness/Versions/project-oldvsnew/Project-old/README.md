# ENGINE DATA SIMULATOR - TX SIDE

This project includes an ICE engine behaviour simulator MCU (TX side) and a data logging + system snapshot MCU (RX side).

The TX side creates data from a single cylinder + one lobe "crankshaft" moving through the combustion cycle and periodically transmits it to another MCU via UART. This project abstracts/oversimplifies a lot of physical/mechanical processes to stay within its scope as a practice program (see Abstractions section). Angular speed range and acceleration/braking/deceleration rates chosen for convenience, not meant to represent real engine behaviour.

The RX side receives the raw data and processes it for two purposes:
	- Text-based display of system snapshots and reports on serial monitor
	- Graphic display of speed and accel/decel rate on serial plotter

This system aims to be portable for future fault injection testing. The RX side includes an error report system using the raw TX data. Errors here are specific situations like faulty response to acceleration or braking, issues with deceleration rate or steady speed mode management. It is silent for now, unless errors on TX side cause unexpected behaviour. Given that the TX side is meant to emulate engine behaviour and not control an engine, it does not have an error handling system for the chosen fault situations.

TX side:
- STM32F4 Nucleo board
- Breadboard
- Joystick
- LED

RX side:
- Arduino Uno:
	- Serial monitor mode: text-based system snapshots + reports
	- Serial plotter mode: graphic visualisation of current angular speed and accel/decel rate

## GOALS
- Practice abstracted, 'ideal world' system control
- Create 'data generation' system to practice communication and data processing systems
- Explicit implementation of FSMs and interrupts

## STRUCTURE
- Simple main.c
- Libraries:
	- Hardware interfaces
	- Data processing/display/transmission
	- Speed regulation functions
	- Timing functions for SYSTEM timing (eg steady speed timer), not interrupt timer
- Src/include:
	- System manager
	- Interrupt handler (incl interrupt timing)
	- Crank module (incl crankshaft system timing ticks)
	- Cylinder head control module
		- Individual cylinder head component modules
		
## BEHAVIOUR
### Input:
- ON/OFF: button
- Acceleration: potentiometer
- 'Braking': joystick

### Control:
'Speed level' is the angular speed of the crankshaft, in degrees per 10 crankshaft system ticks °/(crank_ticks*10).
Unit abbreviated to °/t_c_t.

Idle mode:
- Steady 10°/t_c_t

Acceleration/speed range:
	- Angular speed capped at 1000°/t_c_t
	- Linear acceleration rate:
		- 1°/t_c_t: potentiometer > 0 + hyst (engine idle)
		- 10°/t_c_t: potentiometer > 1023 - hyst
	
'Braking':
	- Defined as direct linear reduction in °/crank_tick, not reduction in vehicle speed
	- Only allowed at > 10°/t_c_t
	- -5°/t_c_t: joystick > ±512 + hyst
	- -100°/t_c_t: joystick > 1023 - hyst
	
Deceleration:
	- Crankshaft stays at steady speed for 10s after accel/braking (system timing)
	- After 10s, speed decreases by -10°/t_c_t

### Output:
- Simple transmission of two bit-packed 32-bit data sets:
	- Raw engine stats:
		- Angular speed (°/t_c_t)
		- If relevant: current accel/decel rate (°/t_c_t)
		- 'Throttle' currently engaged (bool)
		- 'Brakes' currently engaged (bool)
	- Derived stats:
		- Cylinder element timing deltas in crank ticks (eg ticks between exhaust valve close and intake valve open)
		
- LED blink to visually represent speed:
	- 10°/t_c_t -> 1000ms blink pause
	- 1000°/t_c_t -> 10ms blink pause
	
### Error handling:
#### 'Engine' behaviour:
If max allowed speed is reached, system automatically enters pause then deceleration mode until base 10°/t_c_t angular speed.

#### Code execution:
Error response mainly tracks when errors happen by saving an error code and forcing early returns/branch exits if an instruction cannot be executed.

## EXECUTION PATHS
- main.c calls the system manager

- If no interrupt:
	- System manager calls/updates 'crankshaft' and piston module (in src and incl)
		- Crank+piston module calls/updates cylinder head element positions (cylinder manager in src and incl), performs regular routine
	- System manager prepares for next loop
	
- If interrupt happens during detection window:
	- Interrupt handler condition triggers system manager to call updateCrank_Active()
	- System manager sets state to change to ACCEL/DECEL next loop
	
- In the loop after an interrupt event:
	- If there is an interrupt again: same process as before
	- If there is no interrupt:
		- System manager uses tools from lib/timing to start steady speed 10s counter
		- System manager sets state to change to STEADY_ANGULAR_SPEED next loop
		
- Any new interrupt repeats this process and resets 10s counter

- If no interrupt during 10s steady speed hold time:
	- System manager calls updateCrank_Deceleration()
	- System manager sets state to change to DECEL next loop
	
- When the system is in DECEL state AND no interrupt has been detected (= the user is not braking, the system is decelerating on its own):
	- System manager checks decelerationComplete() every loop
	- Once true: system manager sets state to change to IDLE next loop

## ABSTRACTIONS
- Acceleration here is actively implemented by the system manager, and not a mechanical/physical result of throttle input
- 'Speed' here is as angular speed used here to represent the rotational speed of the crankshaft + cylinder system, not the actual speed of the vehicle
- 'Crankshaft' is abstracted into a function that reacts to analog input and drives cylinder sub-system
- This system doesn't have friction/resistance/other source of torque reduction, so lack of throttle input manifests as a coast phase then a linear 'road friction rate' angular speed reduction until back to idle, regardless of starting speed
- Acceleration/braking mechanics highly simplified with linear increases/decreases
- Valves, piston and crankshaft only have two modes: ON/OFF, TDC/BDC, 0°/180°
- Camshaft fully abstracted mechanically, might be implemented symbolically as valve control function
- There is no valve overlap
- Overall simple deterministic system for pure data generation:
	- No backfiring, knock, or other issues
	- Fault injection planned, current project will only implement basic scaffolding
- Overall system mechanics assume an abstract no friction/resistance 'ideal world' model, with no resonance/vibration or thermodynamic effects

## STATE MACHINES
### Main system:
The system has 4 states:
	- Z0 - Idle
	- Z1 - Accelerating
	- Z2 - Steady speed
	- Z3 - Decelerating (by itself or due to braking)
	
#### Z0:
- Z0 -> Z0
- Z0 -> Z1
- Z3 -> Z0

#### Z1:
- Z1 <-> Z1 (until 1000°/t_c_t)
- Z0 -> Z1
- Z1 <-> Z2
- Z3 <-> Z1

#### Z2
- Z2 <-> Z2 (max 10s)
- Z1 <-> Z2
- Z3 <-> Z2

#### Z3:
- Z3 <-> Z3 (until 10°/t_c_t)
- Z1 <-> Z3
- Z2 <-> Z3
- Z3 -> Z0

### Main system state transitions:
UpdateState runs at the start of the loop, using the data from the previous loop
	-> During the rest of a loop, currentState is the results of the conditions established in the previous loop
	-> Current state is then related to previous loop data
	-> Current state reflects the entire set of conditions/data from the entire previous loop (after updatesystem call)
	-> Current state is the state 'base' that will react to all the activity in the current loop

#### Example: System reaches the end of 10s coast time duration and no interrupt is detected
##### Loop x:
- Start of loop:
	- Before any instructions are executed:
		- currentState == Z2
		- bool coastingTimerElapsed == false
	- updateState() called as first instruction
		- -> current state is now Z2 (coasting)
- Z2 state will persist for rest of loop
- No interrupt detected
- Event this loop: coasting time counter reaches 10s
	- -> coastingTimerElapsed set to true
- End of loop:
	- currentState == Z2 still
	- coastingTimerElapsed == true

Loop x+1:
- Start of loop:
	- Before any instructions are executed:
		- currentState == Z2
		- bool coastingTimerElapsed == true
	- updateState() called as first instruction
		- -> current state is now Z3 (deceleration)
		- coastingTimerElapsed reset to false
- Z3 state will persist for rest of loop
- No interrupt this loop
- End of loop:
	- currentState == Z3 still
	- coastingTimerElapsed == false
	
Loop x+2:
- Start of loop:
	- Before any instructions are executed:
		- currentState == Z3
		- bool coastingTimerElapsed == false, does not get altered
	- updateState() called as first instruction
		- -> current state is still Z3
- Z3 state will persist for rest of loop
- No interrupt this loop
- End of loop:
	- currentState == Z3 still
	
Loop x+2 will repeat until engineIdle() == true.
If engineIdle() == true happens on loop x+10, system will enter next loop (x+11) in Z3 and change to Z0 when updateState() gets called.

previousSystemState = currentSystemState; at the end of every successful

### Cylinder:
The cylinder module is a localised Moore-like state machine with 4 states:

Z0 = intake, Z1 = compression, Z2 = combustion, Z3 = exhaust

The cycle is always: Z0 -> Z1 -> Z2 -> Z3 -> Z0 ...

The cylinder manager handles this FSM.

## INTERRUPT
Main system task: idling.
The interrupt event is input from the joystick -> speed regulation.
It has its own timer that 'listens' every 50ms. Lower interrupt listen frequency intentionally chosen to make system less responsive and allow user to move more slowly through speed/braking range while visualising data on serial plotter. The timer resets every 250ms.
The interrupt handler resets the interrupt counter.

## BUILD PROCESS
### Plan:
- Create folders and files to sketch program structure
- Write plans, goals, limitations/abstractions, intended system stucture, behaviour and execution paths
- Draw state machines graph and write down states + state paths

### Draft:
- Start sketching basic code structure in main and manager modules (system, interrupt, crankshaft, cylinder)
- Work from the outside in implementing broad then localised logic
- Move a layer or more up when localised logic points to improvements/changes needed in the layers above
- In parallel, draft parts of error system as the corresponding system behaviour is defined


# ========================= 19/02 REFACTOR =============================
# ENGINE DATA SIMULATOR - TX SIDE

This project includes an ICE engine behaviour simulator MCU (TX side) and a data logging + system snapshot MCU (RX side).

The TX side creates data from a single cylinder + one lobe "crankshaft" moving through the combustion cycle and periodically transmits it to another MCU via UART. This project abstracts/oversimplifies a lot of physical/mechanical processes to stay within its scope as a practice program (see Abstractions section). Angular speed range and acceleration/braking/deceleration rates chosen for convenience, not meant to represent real engine behaviour.

The RX side receives the raw data and processes it for two purposes:
	- Text-based display of system snapshots and reports on serial monitor
	- Graphic display of speed and accel/decel rate on serial plotter

This system aims to be portable for future fault injection testing. The RX side includes an error report system using the raw TX data. Errors here are specific situations like faulty response to acceleration or braking, issues with deceleration rate or steady speed mode management. It is silent for now, unless errors on TX side cause unexpected behaviour. Given that the TX side is meant to emulate engine behaviour and not control an engine, it does not have an error handling system for the chosen fault situations.

### TX side:
- STM32F4 Nucleo board
- Breadboard
- Joystick
- LED

### RX side:
- Arduino Uno:
	- Serial monitor mode: text-based system snapshots + reports
	- Serial plotter mode: graphic visualisation of current angular speed and accel/decel rate

## GOALS
- Practice abstracted, 'ideal world' system control
- Create 'data generation' system to practice communication with data processing system
- Explicit implementation of FSMs and interrupts
- Model simplified, abstract 'engine' mechanics

## STRUCTURE
- Simple main.c
- Libraries:
	- Hardware interfaces
	- Data processing/display/transmission
	- Speed regulation functions
	- Timing functions for SYSTEM timing (eg steady speed timer), not interrupt timer
- Src/include:
	- System manager
	- Error handling
	- Interrupt handler (incl interrupt timing)
	- Throttle and brake input module
	- Engine manager
	- Crank module (incl crankshaft system timing ticks)
	- Cylinder head control module
		- Individual cylinder head component modules
		
## BEHAVIOUR
### Input:
- ON/OFF: button
- Acceleration: potentiometer
- 'Braking': joystick

### Control:
'Speed level' is the angular speed of the crankshaft, in degrees per 10 crankshaft system ticks °/(crank_ticks*10).
Unit abbreviated to °/t_c_t.

Idle mode:
- Steady 10°/t_c_t

Acceleration/speed range:
	- Angular speed capped at 1000°/t_c_t
	- Linear acceleration rate:
		- 1°/t_c_t: potentiometer > 0 + hyst (engine idle)
		- 10°/t_c_t: potentiometer > 1023 - hyst
	
'Braking':
	- Defined as direct linear reduction in °/crank_tick, not reduction in vehicle speed
	- Only allowed at > 10°/t_c_t
	- -5°/t_c_t: joystick > ±512 + hyst
	- -100°/t_c_t: joystick > 1023 - hyst
	
Deceleration:
	- Crankshaft stays at steady speed for 10s after accel/braking (system timing)
	- After 10s, speed decreases by -10°/t_c_t

### Output:
- Simple transmission of two bit-packed 32-bit data sets:
	- Raw engine stats:
		- Angular speed (°/t_c_t)
		- If relevant: current accel/decel rate (°/t_c_t)
		- 'Throttle' currently engaged (bool)
		- 'Brakes' currently engaged (bool)
	- Derived stats:
		- Cylinder element timing deltas in crank ticks (eg ticks between exhaust valve close and intake valve open)
		
- LED blink to visually represent speed:
	- 10°/t_c_t -> 1000ms blink pause
	- 1000°/t_c_t -> 10ms blink pause
	
### Error handling:
#### 'Engine' behaviour:
If max allowed speed/acceleration is reached, system automatically enters pause then deceleration mode until idle 10°/t_c_t angular speed.

#### Code execution:
Program execution error response tracks when errors happen by saving an error code and forcing early returns/branch exits if an instruction cannot be executed. Engine 'error handling' handles system behaviour when speed/accel limit is reached

## EXECUTION PATHS
- main.c calls the system manager

- If no interrupt:
	- System manager calls/updates engine manager
		- Engine manager updates piston+crankshaft module according to current state
			- Crank+piston module updates cylinder head element positions when piston changes between 'BDC' and 'TDC' states (simplified into angle ranges)
		- Engine manager ends, prepares for next loop
	- System manager prepares for next loop
	
- If interrupt happens during detection window:
	- Interrupt handler condition triggers system manager
		- System manager tells engine manager to read new throttle/brake input
			- Engine manager reads+translates input into angular speed regulation control
				- Crank+piston module reacts
					- Cylinder head reacts to crank+piston
	
- In the loop after an interrupt event:
	- If there is an interrupt again: same process as before
	- If there is no interrupt:
		- Engine manager uses tools from lib/timing to start steady speed 10s counter
		- Engine manager sets state to change to STEADY_ANGULAR_SPEED next loop
		
- Any new interrupt repeats this process and resets 10s counter

- If no interrupt during 10s steady speed hold time:
	- Engine manager calls updateCrank_Deceleration()
	- Engine manager sets state to change to DECEL next loop
	
- When the engine is in DECEL state AND no interrupt has been detected (= the user is not braking, the system is decelerating on its own):
	- Engine manager checks decelerationComplete() every loop
	- Once true: engine manager sets state to change to IDLE next loop
	
- System manager just knows when an interrupt happened, actual input data goes from the hardware interface to engine manager via throttle/brake

## ABSTRACTIONS
- Acceleration here is actively implemented by the system manager, and not a mechanical/physical result of throttle input
- 'Speed' here is as angular speed used here to represent the rotational speed of the crankshaft + cylinder system, not the actual speed of the vehicle
- 'Crankshaft' is abstracted into a function that reacts to analog input and drives cylinder sub-system
- 'Braking' here is defined as the opposite effect of 'acceleration' input
- 'Engine manager' here is an equivalent to the physical mechanism tying throttle/brake input to changes in engine load affecting combusion process
- The combustion process is symbolised by the regulation of piston angular speed
- This system doesn't have friction/resistance/other source of torque reduction, so lack of throttle input manifests as a coast phase then a linear 'road friction rate' angular speed reduction until back to idle, regardless of starting speed
- Acceleration/braking mechanics highly simplified with linear increases/decreases
- Valves, piston and crankshaft only have two modes: ON/OFF, TDC/BDC, 0°/180°
- Camshaft fully abstracted mechanically, might be implemented symbolically as valve control function
- There is no valve overlap
- Overall simple deterministic system for pure data generation:
	- No backfiring, knock, or other issues
	- Fault injection planned, current project will only implement basic scaffolding
- Overall system mechanics assume an abstract no friction/resistance 'ideal world' model, with no resonance/vibration or thermodynamic effects

## STATE MACHINES
### Engine system:
The engine has 4 states:
				ENGINE_IDLE,
				ENGINE_INPUT,
				ENGINE_COAST,
				ENGINE_AUTO_DECEL,
				ENGINE_UNDEFINED
	- Z0 - Idle
	- Z1 - Accelerating
	- Z2 - Steady speed
	- Z3 - Decelerating (by itself or due to braking)
	
#### Z0:
- Z0 -> Z0
- Z0 -> Z1
- Z3 -> Z0

#### Z1:
- Z1 <-> Z1 (until 1000°/t_c_t)
- Z0 -> Z1
- Z1 <-> Z2
- Z3 <-> Z1

#### Z2
- Z2 <-> Z2 (max 10s)
- Z1 <-> Z2
- Z3 <-> Z2

#### Z3:
- Z3 <-> Z3 (until 10°/t_c_t)
- Z1 <-> Z3
- Z2 <-> Z3
- Z3 -> Z0

#### State transitions:
UpdateEngineState runs at the start of the loop, using the data from the previous loop
	-> During the rest of a loop, currentState is the results of the conditions established in the previous loop
	-> Current state is then related to previous loop data
	-> Current state reflects the entire set of conditions/data from the entire previous loop (after updateenginestate call at the start of loop)
	-> Current state is the state 'base' that will react to all the activity in the current loop

#### Example: Engine system reaches the end of 10s coast time duration and no interrupt is detected
##### Loop x:
- Start of loop:
	- Before any instructions are executed:
		- currentState == Z2
		- bool coastingTimerElapsed == false
	- updateState() called as first instruction
		- -> current state is now Z2 (coasting)
- Z2 state will persist for rest of loop
- No interrupt detected
- Event this loop: coasting time counter reaches 10s
	- -> coastingTimerElapsed set to true
- End of loop:
	- currentState == Z2 still
	- coastingTimerElapsed == true

Loop x+1:
- Start of loop:
	- Before any instructions are executed:
		- currentState == Z2
		- bool coastingTimerElapsed == true
	- updateState() called as first instruction
		- -> current state is now Z3 (deceleration)
		- coastingTimerElapsed reset to false
- Z3 state will persist for rest of loop
- No interrupt this loop
- End of loop:
	- currentState == Z3 still
	- coastingTimerElapsed == false
	
Loop x+2:
- Start of loop:
	- Before any instructions are executed:
		- currentState == Z3
		- bool coastingTimerElapsed == false, does not get altered
	- updateState() called as first instruction
		- -> current state is still Z3
- Z3 state will persist for rest of loop
- No interrupt this loop
- End of loop:
	- currentState == Z3 still
	
Loop x+2 will repeat until engineIdle() == true.
If engineIdle() == true happens on loop x+10, system will enter next loop (x+11) in Z3 and change to Z0 when updateState() gets called.

previousSystemState = currentSystemState; at the end of every successful

### Cylinder:
The cylinder module is a localised Moore-like state machine with 4 states:

Z0 = intake, Z1 = compression, Z2 = combustion, Z3 = exhaust

The cycle is always: Z0 -> Z1 -> Z2 -> Z3 -> Z0 ...

The cylinder manager handles this FSM.

## INTERRUPT
Main engine task: idling.
The interrupt event is input from the joystick -> speed regulation.
It has its own timer that 'listens' every 50ms. Lower interrupt listen frequency intentionally chosen to make system less responsive and allow user to move more slowly through speed/braking range while visualising data on serial plotter.

### Engine manager:
Receives throttle and brake inputs and converts them into 'engine load' value (in %).
	- -> Piston+crankshaft module converts engine load into 'torque' = angular speed (in °/ten engine system ticks)
		- -> Camshaft module updated by crankshaft at 2:1 gear ratio
			- -> Valve states updated at every camshaft revolution
	- -> Fuel and spark module controls timing using engine and valve states
