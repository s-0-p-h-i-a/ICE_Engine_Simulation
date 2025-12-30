## Development Log — v0

### Scope

Initial implementation and validation of a joystick-controlled “toy ECU” engine simulation, including:

- Engine state handling (on/off, idle, speed levels)
- Cylinder stroke state machine + LED display
- Flywheel + servo actuation
- Hall sensor + RPM calculation
- Early architecture and dependency cleanup

---
## Major Architecture Changes

- Replaced broad enums with `static const` values or local defines to improve containment and reduce cross-module coupling.

- Reduced global state:
    
    - Component states (cylinder stroke, flywheel angle, hall state) are now owned locally by their respective modules.
    - Engine state is defined only by joystick input and latched control logic.

- Clearer separation of concerns:
    
    - Engine is a composite system (drive logic).
    - Cylinder and flywheel are independent state machines driven by engine state.

- Timing is now **module-local**:
    
    - Cylinder display, joystick debounce, hall sensor, and servo timing each manage their own `millis()` usage.

- Replaced persistent globals with accessor functions where values are only needed transiently.

---
## Cylinder System Debug & Validation

### Issues Found

- Cylinder state oscillated rapidly instead of progressing through strokes.
- LED matrix displayed incorrect states or jittered at higher speeds.

### Fixes

- Diagnosed using serial plotter output of stroke state.
- Fixed stuck condition where `switchNow` remained true, causing repeated invalid state transitions.
- Rebuilt cylinder state storage to avoid pointer/array misuse (junk values).
- Synced stroke transitions with LED on/off timing.
- Moved blink timing to `millis()`-based state instead of global counters.

**Result:**  
Cylinder stroke progression and LED display fully validated across idle and speed levels.

---
## Flywheel + Servo Integration

### Early Issues

- Servo wired to wrong pin (exhaust floating).
- Servo jittering or stuck near 90°.
- Flywheel logic scattered across servo and flywheel modules.
### Structural Fixes

- Reversed dependency: flywheel owns angle and motion state, servo only reacts.
- Moved edge-angle handling into flywheel module.
- Split responsibilities:
    - Angle increment
    - Edge handling
    - Direction toggling

---
## Example Debugged Behaviour: Flywheel Angle & Speed Bug

### Observed Fault (via terminal output)

The flywheel would hit 180°, toggle speed, but never sweep back:

```
Current angle: 178
Current speed: 2
Current angle: 180
Current speed: 2
Current angle: 178
Current speed: -2
Current angle: 180
Current speed: 2
Current angle: 178
Current speed: -2
```
### Root Causes

- Angle declared as `unsigned`, breaking decrement logic.
- Speed toggling and angle updates were mixed across functions.
- First-loop initialisation caused invalid branch behaviour.

### Fix

- Converted angle to signed type.
- Centralised angle + speed updates inside `spinFlywheel`.
- Separated:
    - Edge detection
    - Direction toggling
    - Angle incrementation
- Clarified which function owns which part of the flywheel state.

**Result:**  
Correct sweep-back behaviour, stable servo motion, deterministic state transitions.

---
## RPM System Rebuild (Hall + RPM Modules)

- Rewrote hall and RPM logic to remove hidden global dependencies.
- Hall module now owns:
    - Timing
    - Sensor debounce
    - Second-pass detection
- RPM module now performs **pure calculation** using hall-provided data.
- Removed unnecessary engine-state checks from RPM logic.
- Fixed hall jitter by tracking last sensor state.
- Ongoing investigation into rounding and low-speed edge cases during v0.

---
## Status at End of v0

- Engine drive logic validated.
- Cylinder + flywheel + servo behaviour correct across all speed levels.
- Joystick debounce and control stable.
- RPM system rebuilt and validated.
- Codebase refactored for clearer ownership, timing isolation, and future expansion.

**v0 conclusion:**  
Core engine simulation works end-to-end. Remaining work focuses on refinement, documentation, and structured data interfaces (introduced in v1).
