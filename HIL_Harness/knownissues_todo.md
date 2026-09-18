# ================ ABSTRACTIONS / KNOWN ISSUES / TO-DO ================ 
This document contains details on:
- Abstractions in the simulation of engine physics and mechanics
- Known issues/limitations in the current implementation
- General to-do with both immediate and long-term pending tasks

## ======== ABSTRACTIONS ========
The simulation model in this project highly abstracts engine physics and mechanics to keep them within the intended scope: control/test firmware development practice, not 1-1 real-life system simulation.

In its current state, it is a 100% deterministic system with no possible errors (eg backfiring, late fuel injection, etc) or variations (engine bay temperature, electrical circuit jitter, etc).

The logic is meant to simulate, not reflect combustion cycle mechanics. For example, 'acceleration' here is actively implemented by the engine manager, and is not a mechanical/physical result of throttle input feeding the engine more air.

The following list covers some, not all of the simplifications and is meant to illustrate the overall intended abstraction level:

### ==== General engine system ====
- Overall system mechanics assume an abstract 'ideal world' model, with no resistance, resonance/vibration, or electrical or thermodynamic effects
- This engine system does not have gears: 'speed' here is crankshaft angle increments, representing the angular speed of the flywheel
- 'Engine manager' here is a mechanism converting throttle/brake input to changes in engine load
- This system doesn't have friction/resistance/other source of torque reduction, so lack of throttle input manifests as a coast phase then a linear 'road friction rate' angular speed reduction until back to idle

### ==== Engine components ====
Component representations:
- Crank- and camshaft are angle values
- Piston only has 4 states: TDC, BDC, transitional states between TDC and BDC
- Valves, fuel injector, spark plugs only have two modes: OPEN/CLOSED and ON/OFF

### ==== Functional abstractions ====
- Brake and throttle input produce linearly increasing values within the same range
- Braking here is defined as a symmetrical opposite effect to throttle input
- Brake input resulting in engine load variation through added friction, while throttle input affecting load via increase in air flow/pressure, is fully ignored here
- There is no valve overlap
- Only one valve system update happens at each stroke: one valve either opening or closing

## ======== KNOWN ISSUES ========
### ==== Program execution ====
Syntax, variable/function naming mismatches, inclusion, declaration, leftover unused variables, and other issues throughout the program to be addressed at the first overview+compile check.

Possible data flow and execution path control errors may be undetected by compile check but cause issues at run time.

### ==== Simulation ====
#### Timing:
Mismatches between polling and execution timing leading to wrong behaviour are probable
Examples valves and spark+fuel updates:
- A valve staying open too long due to the duration amplification factor for visualisation being too high
- Angle incrementation may cause valve/spark or fuel updates to be missed 
- Threshold angles for on/off are detected and responded to after they are passed, which introduces latency

## ======== TO-DO ========
### ==== Next steps ====
- Finish updating documentation: create condensed devlog, identify+write down relevant issues/limitations missing from this document
- Write header files, define includes structure
- First compile check + debug
- Use datasheet to create simulation profile lib header

### ==== Once program compiles ====
#### Integrate hardware:
- Build TX MCU + peripherals circuit
- Validate hardware input

#### Test and debug existing system:
- Verify and validate engine simulation:
	- Implement data visualisation
	- Run simulation -> identify and solve simulation errors
	
#### Add data transmission module:
- Finish setting up engine data and error statuses API
- Write data processing and transmission modules
- Review, compile test, debug

#### RX MCU (data logger)
- Write, compile, debug code
- Implement data visualisation
- Test transmission sending fixed constant data from TX side
- Validate transmission
- Validate transmission + logging + visualisation of 'live' test plant behaviour

### ==== Ideas ====
- Implement LED blink every time state changes
- Idem but to represent current angular speed
