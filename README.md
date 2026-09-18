# ICE ECU Firmware-Inspired Projects

## Overview

Microcontroller-based applied learning projects using ICE ECU firmware as thematic context. So far all projects represent a 1-cylinder engine. Planned expansion towards a 4-cylinder engine.

---
## Goals:

The main goals are to practice various aspects of embedded systems development:
- Control firmware development
- Completing full development cycles from planning to hardware integration
- Abstracting real-world mechanical processes into code and logic
- Learning and applying C syntax
- Applying software design concepts
- Integrating sensors
- Implementing communications
- Progressive software bring-up
- Structured testing/debugging
- Iterative development
- Implementing state machines
- Isolating hardware logic

---
## Projects

### Complete

[**Chapter 1: Cylinder Visualisation + Flywheel RPM Calculation System**](./Live_RPM):

Joystick-controlled system with 2 main simulations: visualisation of cylinder elements as they move through the 4-stroke cycle, and rising edge signal generation for live RPM calculation.

- 4 sub-systems:
	- Control: user input for on/off and flywheel angular speed
	- Cylinder: breadboard LED display representing piston, valves, spark plug and injector
	- RPM calculation: Hall sensor + standard servo with magnet for reference edge generation
	- Plotter display: system data telemetry
- V0 fully functional, unfinished V1 to practice implementing added complexity

---
### In Progress

[**Chapter 2: HIL Testing Harness Inspired System**](./HIL_Harness):

2-MCU system for generation of ICE ECU data, subsequent UART transmission of bit-packed "engine snapshots" containing engine behaviour data + enum-based error and status codes, and computer-based telemetry.

Progress from chapter 1: explicit implementation of state machines, introduction of embedded software concepts such as event handlers, system managers, enum-based error and status code tracking, error handling execution path design, uniformisation of code structures throughout the system for easier maintenance, debugging, and expansion into more complex systems.

#### TX Side: STM32 user-controlled engine data generation

User turns engine on/off and controls flywheel angular speed. MCU generates engine component data (RPM, camshaft angle, etc), packs it into a byte alongside system status codes, sends to RX MCU.

- Simulated engine components:
	- Crankshaft
	- Camshaft
	- Flywheel
	- Piston
	- Valves
	- Spark plug
	- Injector

#### RX side: Arduino receiver and telemetry visualiser

MCU receives and unpacks bytes, then processes data for visualisation via Arduino Serial Plotter.

Modules:
- Data reception
- Data processing: unpacking and processing for visual representation
- Telemetry: serial plotter visualisation

---
## Repo Structure

ICE_Engine_Simulation/

├─ Live_RPM/ -------------------- Cylinder Visualisation + Flywheel RPM Calculation System

---└─ Tests_and_Drafts/---------- Drafts and test scripts used to validate V0

---└─ Demos/--------------------- Physical system demo GIFs, videos of telemetry visualisation

---└─ V0_Complete/--------------- Fully functional v0

---└─ V1_Unfinished/------------- V1 expansions, intentionally unfinished to progress to HIL system project

└─ HIL_Harness/------------------ HIL Testing Harness Inspired System V0 (ONGOING WIP)
