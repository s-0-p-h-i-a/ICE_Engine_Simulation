# ECU-Inspired Engine Timing Prototype

## Overview

Ongoing simplified ECU firmware-like prototype project. The emphasis is on system design, integration, debugging, and observing system behaviour rather than physical engine simulation.

---
## Goals / Plans:

- Build a simple, deterministic ICE ECU firmware-inspired engine control / behaviour simulation system
- Progressively integrate different sub-systems
- Explore fault injection and CAN communication
- Implement hardware-based behaviour simulations / visualisations

### Planned Sub-systems:
- Cylinder head:
	- Double camshafts
	- Valves
	- Fuel injectors
	- Spark plugs
- Cylinder:
	- Cylinder body
	- Piston
- Crankshaft
- Flywheel

---
## Project Status

### Complete

[**Cylinder + Flywheel RPM Calculation System**](./Main_Project):

Integrates previously separate cylinder and flywheel rpm projects into one system with centralised user control.

- User-controlled via joystick
- 4 main sub-systems:
	- Control: user input for on/off and speed
	- Cylinder: breadboard LED display
	- RPM calculation: Hall sensor + standard servo with magnet for reference edge generation
	- Plotter display: system data telemetry
- v0 fully functional, v1 WIP

### In Progress

[**WIP Folder**](./WIP):

Modules and code that are being worked on. Fully validated elements will be integrated into the main project.

- [V1](./WIP/Main_v1_WIP): ongoing semantic rework, adding in-code comments, localised logic / architecture refactors

---
## Scope & Design Boundaries

This project is intentionally not a physically accurate engine model or a production-grade ECU implementation.

Several design choices differ from real engines and ECUs on purpose, in order to focus on specific learning objectives:

- The “flywheel” is servo-driven and mechanically unrealistic, but deliberately chosen to generate clean, controllable edge signals for RPM measurement and timing logic.
- Hardware complexity is kept minimal to prioritise understanding signal flow, state handling, and module interaction over component realism.
- Timing, actuation, and sensing are simplified to allow step-by-step validation and debugging without relying on large external libraries.
- Safety-critical, real-time, and performance constraints of production ECUs are out of scope for this prototype.

The goal is to explore system structure, data flow, and incremental integration of engine-related sub-systems, while building a foundation for more realistic models and hardware in future iterations.

---
## Repo Structure

ICE_Engine_Simulation/

├─ Main_Project/ -------------------- Fully validated modules

---└─ Tests_and_Drafts/------------ Drafts and test scripts used to validate v0

---└─ V0_Complete/---------------- Fully functional v0

└─ WIP/----------------------------- Work-in-progress modules

---└─ Main_v1_WIP/----------------- Ongoing v1 rework, refactors, comments

---└─ v1_Crank_and_Restructure/--- Draft for crankshaft module integration and architecture changes
