# Engine Data Simulator — Condensed Development Log

---

## Phase 1 — Initial architecture and first working structure
### 16–17 February

The first working version established the core engine on/off mechanism, a basic state machine, and the initial error handling approach. The main focus was defining how the system should manage state across loops and what responsibilities belonged where. Several early design decisions were revised almost immediately once their limitations became clear under more complex conditions.

#### Engine manager
- Introduced `engineON` bool and `ENGINE_UNDEFINED` as an initialisation sentinel so the system always starts in a defined state before the user turns the engine on
- Designed the loop closure routine (`finishLoopRoutine`) and the next-loop preparation step (`prepareNextLoopState`) as distinct functions with different responsibilities: one handles execution status and reset, the other prepares state transition conditions
- Decided that state memory should be baked into the FSM logic rather than held in a separate `lastSystemState` variable — the current state dictates which transition conditions apply
- Identified that `changeStateNextLoop` was being assigned in two places; centralised it to `prepareNextLoopState` only
- Removed `lastSystemState`, renamed `currentSystemState` to `systemState` to avoid confusion about what "current" means across loop boundaries
- Established the principle that deeply nested function calls should not all carry error tracking — innermost compute/assign functions stay simple, with error catching only at meaningful execution boundaries

#### Error handling
- Initial approach: all error enums centralised in the error handling module
- Identified early that interrupt detection and system execution status were being conflated — decoupled them: interrupt detection is a state event, system status belongs to error handling
- Decided to be selective about enforced error returns: if a called function already handles its own error status, the caller can exit normally

#### Crank and piston
- Removed `angularSpeed` as a separate concept, establishing that angular speed and angle step change are equivalent at this abstraction level
- Decided that `updateCylinder` gives the crank angle and a timer phase flag to the cylinder, which responds to angle without knowing anything about time
- Removed `current` prefix from variables to avoid confusion about what "current" means at loop boundaries

---

## Phase 2 — Major refactor: system manager / engine manager split
### 19 February

A significant structural refactor separated what had been a combined system-and-engine manager into two distinct layers. This was driven by the recognition that combined concerns were creating unnecessarily complex logic and making state management harder to reason about.

**ICE topic:** The refactor's data flow — throttle/brake input → engine load → crankshaft angular speed → cylinder head updates — was explicitly modelled on the real physical chain from driver input to drivetrain response.

#### System manager
- Reduced to a single responsibility: detecting whether a hardware input event occurred and passing that information to the engine manager
- Removed FSM — the system manager has no states beyond "interrupt happened / no interrupt"
- Loop status tracking simplified or removed as responsibilities moved to the engine manager

#### Engine manager
- Took ownership of the FSM, all engine state logic, and input processing
- Engine states redefined from idle/accel/decel/coast to `ENGINE_IDLE`, `ENGINE_INPUT`, `ENGINE_AUTO_DECEL`, `ENGINE_COAST` — more accurately reflecting the control logic intent
- Engine load set to 0–100% scale; throttle/brake input translated into percentage point changes; idle load set at 20%; deceleration rate set to 10% per 10 loops
- Engine manager now only computes engine load — the crank module translates that into angular speed

#### Crank and piston
- Established that the crank module receives engine load and is solely responsible for translating it to angle step changes
- Removed state enum from crank module — crank does not need to know engine state, only engine load
- TDC/BDC toggle implemented as piston edge state detection

#### Error handling
- Continued loosely centralising error codes in the error handling module with the explicit intention to separate and localise them later
- Noted the intended cascading error code architecture: engine manager can surface cam update errors, cam can surface valve errors

---

## Phase 3 — Camshaft introduction and physical modelling
### 24 February

This phase introduced the camshaft as a real simulation module, moved stroke state ownership to the cam module, and built the angles library. The physical 2:1 gear ratio between crankshaft and camshaft was implemented explicitly.

**ICE topics:** Camshaft drive ratio (2:1 crank-to-cam), valve timing defined by cam angle ranges rather than abstract stroke indices, the four-stroke cycle as the driver of valve state transitions.

#### Camshaft
- Introduced as a distinct module replacing the previous "cylinder" abstraction
- Cam angle derived directly from crank angle with a 2:1 ratio — updated every 2 crank angle steps to keep gear ratio division results as integers
- Cam update call placed in the crank module, reflecting the physical mechanical relationship
- Stroke state ownership moved to the cam module: camshaft revolution completion defines stroke phase transitions
- Removed the `% 2` angle update condition in favour of an explicit threshold-crossing approach for valve control
- Decided to use a threshold angles array rather than a verbose switch branch for valve transitions — advance in stroke phase and valve toggle identified as two different event types

#### Crank and piston
- Split `updateCrankSystemTicks` into separate update and check functions (`checkTickerPhaseElapsed`)
- `checkTickerPhaseElapsed` called at the start of `pushPiston` — causes early return if the 10-tick phase has not elapsed, acting as a local angle update debounce
- Restructured `updatePistonState` to use early returns instead of an if-else chain; now returns piston state rather than modifying it in place
- Moved `wrapAngle` to a static local function in the angles library

#### Angles library
- Created to share angle typedefs, wrap logic, and update functions between crank and cam modules
- `updateAngle` now incorporates wrap behaviour internally

#### Interrupt handler
- Identified that `HAL_GetTick()` values are not synchronised with loop iterations — a modulo-based phase check (`ticks % PHASE == 0`) could miss the exact tick entirely between consecutive loop calls
- Switched to a delta-time pattern: store `lastPollTime`, compare `timeNow - lastPollTime >= TIMER_PHASE`
- Single hardware read on event detection implemented to avoid reading input twice at different points in the same loop

#### Valve control / fuel and spark
- Separated shared stroke cycle library for use as array indices in both modules
- Valves and fuel/spark lines keep local stroke state — they are not mechanically linked and should own their own state independently

---

## Phase 4 — Fuel, spark, and stroke ownership refinement
### 27 February

Resolved the question of who owns stroke state, placed the fuel and spark update call at the end of the engine manager function, and updated the README with the full input-to-output dynamics description.

**ICE topics:** Fuel injection and spark ignition controlled by ECU based on engine state data; stroke cycle progression driven by camshaft revolution.

#### Fuel and spark lines
- Stroke state ownership question resolved: camshaft revolution drives stroke cycle progression, so the cam module owns stroke state; fuel and spark lines access it via the engine API
- Fuel and spark update placed at the end of `runEngineManager` so the module always works with fully updated engine state data

#### Error handling
- Removed silent error handling from valves and fuel and spark modules — these modules only assign values and have no meaningful error surface

---

## Phase 5 — Full structural review: execution paths and dependencies
### 22–23 April

A systematic review of the entire codebase working from main inward, focused on execution path clarity, module responsibilities, and dependency structure. Several conceptual changes were made alongside code-level fixes.

#### Main
- Redesigned as a dumb driver: initialises the system once, runs the system manager in a loop, knows nothing about engines or hardware
- Removed all error handling logic from main — system manager is responsible for error detection and execution decisions
- `systemInit` moved back to main so it can gate further execution; system manager no longer needs to check init status each loop

#### System manager
- Separated init statuses (`HARDWARE_INIT_ERROR`, `ENGINE_INIT_ERROR`, `HW_AND_ENGINE_INIT_ERROR`) from runtime system statuses (`ENGINE_MANAGER_ERROR`, `SEND_DATA_ERROR`)
- `hardwareInitOK` and `engineInitOK` booleans added for granular init failure tracking
- `runSystemManager` returns bool to allow main's while loop to exit on unrecoverable error
- `newInputThisLoop` made a local variable inside `runSystemManager` — removes the implicit dependency on a variable defined in the engine manager module
- Getter return types changed from `bool` cast to the actual enum types (`SystemStatuses`, `InitStatuses`) for correct data log packing
- Decision recorded: system manager status represents the last point of execution failure; each sub-module owns its own detailed error codes, which are packed into the data log independently

#### Event handler (formerly system interrupt)
- Renamed from interrupt handler to event handler — the module polls for input on a timer, it does not handle hardware interrupts; the name change removes a misleading embedded systems connotation
- `lastPollTime` initialisation changed: `HAL_GetTick()` cannot be used as a static initialiser (not a compile-time constant) — initialised to `TIMER_ZERO` instead
- `lastPollTime` assignment moved inside the `if (phaseElapsed)` branch so the reference time resets only when a poll actually fires, making the 50ms poll interval measured from the last poll rather than from every loop
- Functions collapsed to `eventDetected()` and `listenNow()`; event handler state getter added for data log

#### Engine manager
- Engine toggle handling moved before the state update switch — toggle overrides all states and must be applied before the FSM evaluates conditions for the current loop
- `handleToggleInput` cleaned: flips `engineON`, calls `resetToIdle` if on, calls `engineSystemInit` if off
- `ENGINE_UNDEFINED` and `ENGINE_OFF` separated: undefined is a genuine error/unknown condition, off is a valid operational state with defined behaviour
- `default` case in `updateEngineState` now returns 0 (error) since all valid states are explicitly handled
- Split into `engine_manager.c` (FSM, state update, run function) and `engine_manager_helpers.c` (init, reset, input processing, load computation, getters)
- `computeNewLoad` extracted as its own function
- `changeStateNextLoop` variable removed — no longer used

#### Throttle, brake, ignition
- Module renamed from `throttle_and_brake` to `throttle_brake_ignition` to include ignition toggle
- Throttle and brake inputs clamped independently before being combined into net engine input
- Hardware input to engine load conversion ratio adjusted: analog max 1023 divided by 102 gives a max step of 10° per update, preventing cam angle threshold crossings from being skipped in a single loop

---

## Phase 6 — Cam, valves, and fuel/spark timing rework
### 23–24 April

Redesigned the camshaft, valve control, and fuel and spark modules to use angle-based timing rather than stroke-indexed arrays, making the physical timing relationships explicit in the code.

**ICE topics:** Valve timing defined in crank degrees; injection timing 10–30° BTDC; ignition timing 15–35° BTDC; spark duration ~0.6–3ms; injection duration ~1ms; camshaft at half crankshaft speed.

#### Camshaft
- Replaced stroke-indexed array approach with angle threshold crossing detection: `camshaftAngleLastLoop` and `camshaftAngleThisLoop` used to detect when a threshold angle has been passed
- `newHalfRevComplete()` checks for cam angle wrap to detect half-revolution completion and advance stroke
- `toggleValveNow()` runs every loop independently of stroke advance, checking angle thresholds to trigger valve state changes — stroke advance and valve actuation confirmed as two separate event types

#### Valve control
- Rewrote from stroke-indexed state arrays to a switch on `currentStroke`, updating individual `intakeValveState` and `exhaustValveState` variables
- Each case makes the physical valve event explicit: exhaust closes at `INTAKE`, intake closes at `COMPRESSION`, exhaust opens at `COMBUSTION`, intake opens at `EXHAUST`
- Stroke state ownership: `currentStroke` owned by camshaft module, passed to valve control as the index for which valve event to execute

#### Fuel and spark lines
- Reworked from stroke-on/off arrays to angle-based timing with explicit duration
- Timing angles set as constants: `INJECTION_TIMING_ANGLE = ANGLE_TDC - 20`, `IGNITION_TIMING_ANGLE = ANGLE_TDC - 25`
- `FuelOrSparkElement` struct introduced to bundle element state, timing angle, and duration — eliminates repetition and provides a natural fault injection hook (modifying `timingAngle` in the struct changes timing without touching control logic)
- Duration tracking implemented as an FSM: `ELEMENT_IDLE` → `ELEMENT_ACTIVE` → `ELEMENT_IDLE`, replacing a blocking while loop that was stalling the main loop execution
- `crankAngleLastLoop` / `crankAngleThisLoop` pattern added, mirroring the camshaft threshold detection approach
- Module now includes getter headers from crank and cam modules directly rather than going through the engine API — fuel and spark only needs stroke and crank angle, and the API is reserved for data transmission only
- `DURATION_AMPLIFY` multiplier added to extend on-time for serial monitor visualisation

#### Crank and piston
- Replaced internal loop-tick counter with `HAL_GetTick()` delta-time pattern — loop ticks have variable duration, making `°/N ticks` physically inconsistent; `°/~10ms` is more accurate
- Piston state extended beyond TDC/BDC to include intermediate states, managed by switch branch
- Load-to-angle-increment conversion ratio introduced — previous version was incrementing by total engine load directly rather than a proportional angle step

---

## Phase 7 — Fuel/spark FSM finalisation and coast timer fix
### 25–26 April

Minor but important corrections to the fuel/spark state machine and the engine manager coast phase.

#### Fuel and spark lines
- Blocking while loop confirmed as unsuitable — replaced with non-blocking if-check FSM managing `ELEMENT_IDLE` / `ELEMENT_ACTIVE` states and a `lastTimeSwitchedOn` timestamp for duration tracking

#### Engine manager
- Found that `coastTimeElapsed()` was being called but `startCoastTimer()` had never been written — added `startCoastTimer` to trigger on the `ENGINE_INPUT` → `ENGINE_COAST` transition
- Coast phase duration set to 10 seconds
