# README — v0

## Scope of v0

Version 0 represents the **first fully functional and validated end-to-end implementation** of the mini engine / RPM simulation.

All core subsystems are implemented, integrated, and verified working together:

- Joystick-driven engine control (on/off, idle, speed levels)
- Flywheel reference generation using servo actuation
- Hall sensor edge detection and RPM calculation
- Cylinder state machine with LED-based visualization
- System timing using `millis()` (no blocking delays)

v0 establishes a **working baseline** before architectural refinement and data-layer separation introduced in later versions.

---
## What Was Achieved

- Complete control loop: **input → control → actuation → sensing → calculation → display**
- Independent, module-local timing for:
    - Joystick debounce
    - Servo movement
    - Cylinder display
    - Hall sensor sampling    
- Reduction of global state and clearer ownership of variables per module
- Successful integration of physical hardware system with firmware logic

All major behaviors were validated through:

- Hardware behaviour
- Terminal output
- Serial plotter visualization
- Manual cross-checking of RPM behavior

---
## Status at End of v0

- System is **fully functional and stable**
- All core behaviors work as intended across speed levels
- Codebase is structurally sound but still “prototype-shaped”
- Clear opportunities identified for improving readability, semantics, and layering

v0 serves as the **validated foundation** for v1, where the focus shifts to:

- Semantic cleanup
- Data interface layers
- Improved safety and readability
- Better separation between control logic and visualization
