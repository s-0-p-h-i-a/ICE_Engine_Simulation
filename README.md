# ECU-Inspired Engine Timing Prototype

## Overview

Ongoing simplified ECU firmware-like prototype project. The emphasis is on system design, integration, debugging, and observing system behaviour rather than physical engine simulation.

---
## Goals / Plans:

- Build a simple, deterministic ICE ECU firmware-inspired engine control/simulation system
- Progressively integrate different sub-systems
- Explore fault injection and CAN communication
- Implement hardware-based behaviour simulations and visualisations

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

### Complete:

[**Cylinder + Flywheel RPM Calculation System**](https://github.com/s-0-p-h-i-a/ICE_Engine_Simulations/tree/main/Full_Engine/Cylinder_and_Flywheel):

Integrates previously separate cylinder and flywheel rpm projects into one system with centralised user control.

- User-controlled via joystick
- 4 main sub-systems:
	- Control: user input for on/off and speed
	- Cylinder: breadboard LED display
	- RPM calculation: Hall sensor + standard servo with magnet for reference edge generation
	- Plotter display: system data telemetry
- v0 fully functional, v1 WIP
