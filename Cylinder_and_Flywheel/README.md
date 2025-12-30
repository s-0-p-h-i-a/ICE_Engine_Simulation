# Cylinder + Flywheel + RPM Calculation Modules

## Overview

This system focuses on engine timing, RPM calculation, and cylinder visualization.
It is made of the following sub-systems:

- **Control**: takes user input via joystick for engine on/off and speed level
- **Cylinder**: state machine / actuator and breadboard LED display of cylinder elements
- **RPM**: standard servo "flywheel" for reference edge generation, Hall sensor and RPM math
- **Telemetry**: system data visualisation via serial plotter

The system is designed to be incrementally extensible, with clear separation between control logic, hardware interfaces, and display/logging layers.

---
## Hardware

* Arduino MCU (ECU side)
* Flywheel (servo with a magnet for RPM reference edge)
* Hall sensor for RPM calculation
* Coloured breadboard LED matrix for cylinder visualization
* Joystick for control

---
## Project Status

Fully integrated in v0, ongoing rework in v1.

- [**V0**](./V0_Complete): fully functioning version, verified and validated hardware integration. Documentation coming soon
- [**V1 (WIP)**](./V1_WIP): ongoing semantic rework, adding in-code comments, and localised logic / architecture refactors

---
## Behaviour:

* User turns system on/off via joystick button click (digital Z)
* "Speed"/RPM setting chosen via analog joystick X axis:
	* Neutral	: idle
	* Up		: speed 1
	* Down 	: speed 2
* Speed mode is interpreted for "flywheel" and cylinder sub-systems:
	* Flywheel: rotating movement 0° - 360° -> "speed" = angle increment
	* Cylinder: on/off blink cycles -> "speed" = blink on/off timing

| Speed Modes |   Flywheel/Servo   |   Cylinder    |
| :---------: | :----------------: | :-----------: |
|    Idle     | 2° angle increment | 1000ms on/off |
|   Speed 1   | 4° angle increment | 550ms on/off  |
|   Speed 2   | 8° angle increment | 100ms on/off  |

---
## Code Architecture

**v0 and v1:**
* **Flywheel module**			: tracks angle and speed
* **Hall sensor module**		: provides timing events
* **RPM calculation**			: converts timing data to speed
* **Cylinder module**			: computes cylinder state
* **Cylinder visualization**	: breadboard LED output
* **Joystick module**			: reads joystick input
* **Drive module**				: on/off and speed control for the flywheel and cylinder
* **Plotter module**			: displays data on serial plotter

**v1 only:**
* **Plotter interface modules**	: expose processed subsystem data for plotting and diagnostics.

Modules are designed to be testable in isolation, with integration exposing timing and logic issues for refinement.

---
## Technical Challenges

See [Technical Challenges](./Technical_Challenges.md) for detailed report of issues solved during development.

This project involved both hardware and firmware challenges, including:

* Reliable servo power and incremental actuation testing
* Hall sensor integration and RPM timing logic
* Cylinder state initialization and timing synchronization
* Servo sweep logic and edge-case handling

---
## Next Steps

* **Fault injection**: timing lag, jitter, dropped events, snapshot logging
* **Expanded engine structure**: crank + cylinder head module with DOHC, valves, injectors, plugs
* **CAN integration**: second MCU handles user input and logging

Additional documentation, diagrams, demos, and design notes will be added as the project evolves.
