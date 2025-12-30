## Project Overview — v1 WIP

This project is a **modular, microcontroller-based toy ECU / engine control and behaviour simulation**, built to practice embedded firmware architecture, hardware–software integration, and test-driven iteration.

It models a simplified engine system with:

- Flywheel reference generation
- Hall sensor–based RPM calculation
- Servo-driven actuator control
- LED display representing a cylinder moving through the combustion cycle
- Joystick-based user input
- Real-time data visualization via plotter output

The system is designed to be **incrementally extensible**, with clear separation between control logic, hardware interfaces, and display/logging layers.

---
## Version Status:

### Partially complete:
- .cpp files: first rework complete, compile check not complete yet

### To-Do:
- .h files: update, compile check
- main .ino: update, compile check
- hardware integration and testing

---
## What Changed in v1

Version 1 introduces a **structural and semantic refactor** over the initial prototype (v0):

### Architecture & Semantics

- Clear, consistent **module semantics** across the codebase (state, update, control, accessor roles).
- Improved **in-code documentation** to reflect intent, data flow, and responsibilities.
- Explicit layering between sensing, control, and presentation.

### Data Interfaces

- Added dedicated **plotter data interface headers**:
    
    - RPM system data interface
    - Drive + cylinder system data interface

- Plotter module now depends **only on data interfaces**, not on internal subsystem logic.

### Control & Safety Logic

- Refined **flywheel and servo logic**, including:
    
    - More explicit state handling
    - Additional safety constraints while preserving intended behavior

- Angle and state handling clarified to reduce invalid or undefined states.

### System Update Flow

- Centralized value refresh via system-wide update functions, ensuring:
    
    - Deterministic update order
    - Single update per loop
    - Clean separation between updating and reading values

---
## Status & Next Steps

This README is intentionally brief.  
More detailed documentation, diagrams, and test notes will be added later using the in-code development diary.

Planned extensions include:

- Fault injection and failure-mode testing
- Additional engine subsystems (e.g. crank and cylinder head modules)
- Expanded visualization and logging support
