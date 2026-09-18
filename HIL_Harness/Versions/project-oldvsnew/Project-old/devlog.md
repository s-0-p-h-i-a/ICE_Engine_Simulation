

=====================================================================================================================
#DEVELOPMENT NOTES

## 16/02
- I want to have engine ON/OFF mechanism without having to implement an 'engine off' state
- Using local engineON state bool
- Added engine undefined state in enum just so system state initialises with a defined value when engine is off
- Engine ON check acts as a sort of preemptive 'interrupt': if engine not on, do not even start main task (idling) and keep jumping to next loop until ignition = button press detected

## 17/02
- Implementing 'memory' in the system? Via a lastsystemstate var or procedurally?
	- Prefer to bake it in the logic, might have a lastsystemstate as temp var while drafting
	- Current idea: updatesystemstate uses switch currentsystemstate to reassign currentsystemstate
- State update process:
	- Currently using this in helper function (preparefornextloop) to prepare system state update logic:
	
	if (getSystemStatus() == INTERRUPT_DETECTED) {
		interruptHappened = 1;
	} else {
		interruptHappened = 0;
	}

	- But what if an interrupt is detected an then an error happens later?
	- With the current structure:
		- preparefornextloop does not get called if an error happens after the interrupt is detected in that loop
	- Generally, what behaviour do I want after a loop that had an error?
		- -> If engine ON, reset to ENGINE_IDLE
		- Add state variable for if loop was executed properly or if it returned early due to error

- Currently error handling module has the code for main.c, system manager and crankshaft system errors
- Localise error enums?
- setSystemStatus(INTERRUPT_DETECTED); when interrupt detected, but setsystemstatus is also used to finish the loop routine
	- -> INTERRUPT_DETECTED would be overriden
	- Local interruptHappened bool?
	- Decouple system status tracking from interrupt event tracking
	- Interrupt is related to state, not to program execution status
	- System status belongs to error handling module, maybe rename module to status manager?
- Engine ON now detected/assigned in the loop closing routine instead of at the start of the loop
	- -> information needed for updatestate at the start of next loop ie at the next instruction between loop exit and start
- Having to be selective about where to enforce error tracking, forcing return 0 anywhere a function could execute wrong would be too much
- If a function already calls a function with an error status update + forced return, the 'outer' function can exit normally
- Ideally if nested function calls, the innermost function is simple compute/set/get/assign and would not be a bool with error tracking and return 0
- Example in preparefornextloop (called right before closing a loop, whether error or not):

if (!checkIfEngineON()) {
		systemInit();
	}
	
systemInit:

bool systemInit(void) {
	setSystemStatus(SYSTEM_OK);
	currentSystemState = ENGINE_UNDEFINED;
	
	if (!crankshaftInit()) {
		setSystemStatus(CRANKSHAFT_INIT_ERROR);
		return 0;
	}
	return 1;
}

- Crankshaft will already track the possible execution error + it is a simple value assignment function
- Systeminit stays as a bool with early return as it is used that way for the initial system setup to prevent system manager from starting. If it is called in system manager scope, then it was already validated when it was called in main before runsystemmanager
- Local loop status tracking for system manager module

- Working on static helpers inside system manager module as their dynamics get more concretely defined
- if engine off -> reset system via systeminit
- Removed lastsystemstate, renamed currentsystemstate to just systemstate as 'current' can induce confusion due to state persisting while one loop ends and the other starts
- engineON update removed from main system manager function, now happens in finishlooproutine
- engineON moved to local var inside finishlooproutine
- engineON var removed, now only engineON() call (it was using a function call to update anyway
- engineON() will use info from input processing interface, that will locally track whether on/off button has been toggled via the hardware interface (using buttonPressed())
- latestLoopStatus var named like this to reflect that it is the value of the latest status reassignment
- Trying to 'untangle' and isolate different system management tasks and responsibilities across functions to keep things clear, eg finishLoopRoutine and prepareNextLoopState are related but have different tasks in different domains
- coastingTimerElapsed changed from local var to function call to coastingTimerElapsed() in the timing lib
- changeStateNextLoop getting assigned in 2 different places: need to centralise this
	- Current situation:
	
	static bool prepareNextLoopState(void) {

	if (getSystemStatus() == INTERRUPT_DETECTED) {
		interruptHappened = 1;
	} else {
		interruptHappened = 0;
	}
	
	changeStateNextLoop = interruptHappened || coastingTimerElapsed();
	}

	static bool finishLoopRoutine(SystemStatuses currentStatus) {
		
		setSystemStatus(currentStatus);
		latestLoopStatus = LOOP_COMPLETE;
		
		if (!engineON()) {
			systemInit();
			changeStateNextLoop = 0;
		}
		
		if (currentStatus != SYSTEM_OK) {
			resetToIdle();
			changeStateNextLoop = 0;
			latestLoopStatus = LOOP_ERROR;
		}
		prepareNextLoopState();
	}

- Now only changeStateNextLoop reassignment is in prepareNextLoopState: it makes more sense there and either way the conditions in prepareNext... are the only things that should trigger a state change
- Trying to build updatestate() logic so that it does not need to know the previous state, the previous state dictates which instructions/conditions apply to assign the new state
- Had previously sketched a bool interruptHappenedThisLoop() function, not necessary anymore
- Switch branch to update system state not needed anymore
- getinterrupttype returns true if acceleration (potentiometer input) and false for deceleration (joystick input detected)

- Step change according to defined goals not implemented, defining the dynamics in crank module first
- Angular speed and angle step change math + correlation to be defined/tightened once process is defined
- Are they basically the same? Yes?
	- Can I remove angular speed type/vars/etc and still do what I plan on doing? Eg 'control' crank and enforce speed limit? Yes -> they are the same -> remove angular speed var, angular speed <-> angle step correlation to remain in program description and outline of equivalent real life processes each system is modelling
- What I wanted was: angle step and angle step change rate

- updateCylinder logic: gives updated crank angle and whether 10 tick phase elapsed to cylinder, cylinder will update its state accordingly -> cylinder does not see or know time, it just responds to crankshaft angle based on its current state
- For now: not really implementing error handling system in crankshaft module as I am still figuring out the logic and execution flow
- Rename crank_piston to just crank and have piston just represented as internal object?
- Removing 'current' prefix from vars as it is confusing re: what 'current' actually is and they all get updated when relevant anyway

- updateCrank functions array or pass state value to crank module? For now: keeping things separated to avoid having to track data flow


# ========================= 19/02 REFACTOR =============================

Refactoring to introduce new system design:
	- Main and system manager drive loopwise updates, system manager detects interrupt
	- System manager 'drives' engine manager
	- Interrupt detected -> system manager tells engine manager to read hardware input
	- Engine manager reads hardware input via throttle and brake
	- Hardware input is translated into 'combustion' process changes = piston 'force'
	- Piston 'force' = crankshaft angular speed
	- Crankshaft rotation updates cylinder head elements
	
So the system manager does know know any specifics about input or engine system regulation, it only knows when an interrupt happened.

## REFACTOR PROCESS:
- Start by copying system manager code to engine manager, split up between modules as relevant
- Using temporary local vars to sketch behaviour, then abstract into procedures
- Using a finishlooproutine for both system and engine manager?
- Probably just engine, putting a copy of it in sys manager module for now
- How much should engine state react to system manager state? Eg if sys manager has fault
- Splitting up system and engine manager is making it possible to remove+simplify a lot of code since concerns are more separate = less combined situations to handle at once
- Also a lot less local constants and variables needed
- Engine manager now only knows engine load (result of input), crank+piston module translates it to speed

### System manager:
- Real FSM now in engine manager
- System manager does not have states beyond interrupt happened/no interrupt
- Keeping loop finished statuses there
- 'Secondary' states: loop finished ok/loop finished with error for error handling
- No, also remove loop statuses as there is so little going on


### Engine manager:
- Input = throttle input (always > 0) + brake input (always < 0)
- Engine has FSM(idle, accel, etc) and system statuses (engine system ok, engine update error)
- Reducing helper functions needed:
Previously used:

static bool prepareNextLoopState(void) {

	if (getManagerState() == INTERRUPT_DETECTED) {
		newInputDetected = 1;
	} else {
		newInputDetected = 0;
	}
	
	changeStateNextLoop = newInputDetected || coastingTimerElapsed();
}

Now:

only using changeStateNextLoop = newInputDetected || coastingTimerElapsed(); in finishengineroutine, as interrupt response is now centralised inside runenginemanager

- Redefining engine states: idle, accel, decel and coast -> idle, input, auto decel and coast 
- Reduced previous code snippet even more: no more changestate... var
- Engine manager now only knows engine load (result of input), crank+piston module translates it to speed
- Set idle load to 20%, throttle/brake input now translated into percentage points change for engine load
- Deceleration rate set to 10% per 10 loops

### Error handling:
- Currently centralising everything in the error handling module, will separate and localise as relevant later

### Crank and piston module:
- Implementing engine load info -> how to process into angle step change
- Engine load = 100 = angle step (per 10 ticks)
- Implement TDC/BDC toggle
- Keeping array of funcs for now, trying to avoid repetition and clearly define engine manager response
- Still repetitive, have internal array of funcs for state dependent tasks and have engine manager just call a generic updatecrank
- States enums in engine and crank+piston modules, using states as value for engine -> crank control
	- Centralise state enum in crank+piston? Could make sense logically: angular speed is what is reacting to the input changes, not the engine itself. Engine holds the 'path' from throttle/brake to angular speed regulation
- But engine manager needs the states more. Make a shared states_lib.h?
- Also, conceptually crank 'states' doesnt really make sense unless they are referring to angles
- Actually crank+piston doesnt need to know about state anyway, only engine load
- Implementing piston 'edge' states: when it reaches 0 or 180 -> update cylinder state
- Piston 'force' drives cylinder system motion/updates
- Ticks math, vars etc might be moved to own lib

# ========================= 24/02 =============================

## Crank and piston module:
- System ticker currently silent, needs to be used to implement °/ten ticks speed unit
- Currently updateCrankSystemTicks both updates ticker and checks if the 10 ticks phase is elapsed
	- -> Split into two with checkTickerPhaseElapsed
- Where to put the ticker update call?
	- It is an internal timer of the engine manager module
	- It is not the result of any input or system effect
	- It affects how often the angle is incremented by the current angle step
	- It acts a sort of throttle input debounce to avoid accelerating too fast
	- Essentially it means the crankshaft angle is only updated every 10 ticks
	- -> Ticker must allow a crank angle update every 10 ticks AND 'mute' crank updates during other ticks
	- checkTickerPhaseElapsed need to be called after crank angle update and set a condition for the angle update to happen or not
	- -> Ticker function called at the start of updateCrank
	- checkTickerPhaseElapsed called in pushpiston, causes early return 1 if phase not elapsed (local piston 'debounce')
	
- For updatePistonState: use local crank angle var or pass it as arg?
- Crank angle var represents current crank state, maybe avoid using it directly when not computing and updating its new value -> pass as arg
- Restructured updatePistonState: returns piston state, ifs with early returns instead of if-else chain
- In pushpiston: updateCamshaft fails silently, handled locally in cam module
	
## Camshaft module: (formerly cylinder module)
- Current structure: cylinder.c as 'manager', individual files for injectors, plugs, valve control
- Idea: implement 'camshaft' for valve control? 2 crank revolutions = 1 cam revolution
- Use states map for cam/valve control?
- Crank updates cylinder every time piston reaches TDC and BDC
	- -> every 2 BDC states, update cam
- Update cam directly from crank module, not cylinder, to mimic real life system more closely
- Crank updates cam with 'debounce' or cam debounces locally?
	- -> local debounce in cam
- Cam similar to crank, state represented by current angle
- -> Just send raw angle to cam module (every 10 ticks debounce) and let cam state be handled locally
- Need to reuse typedefs, functions, constants in both crank and cam
	- -> Make angles lib
- updateCrankAngle currently both updates angle and checks for piston state -> separate into two, raw angle update goes in angle lib
- Wrap angle now static and local to angles lib, incorporated into updateangle
- Angles lib not needed as i can just update cam directly with the updated and processed crank angle
- Updating cam angle every 2 crank angles to have only integer /2 gear ratio division results
- Putting cam update call in crank module as crank wheel drives cam wheels

## Error handling:
- Aiming for clean layering and how to further implement it
- Instead of local statuses for engine elements, general engine status?
- What is the point of engine status?
	- Confirm that everything ran ok
	- OR point to the point of failure
- Currently loosely implementing error codes and dumping everything error related into error handling module, will sort later according to how many error statuses exist for which modules
- Local binary system ok/system error statuses for smaller modules and more detailed process statuses for engine?
- Some error handling mechanisms may be silent/always be at OK, still training where and how to implement error catching and error code updating
- Cleaned up/localised error codes a bit

## Valve control / fuel and spark lines:
- Making lib for 4 stroke cycle stuff that they both need as array indexes to update states
- Centralised 'stroke' state? Am using strokes symbolically for state array indexing
	- Valves and fuel+spark not mechanically linked, use local states
	
## Interrupt:
- How to detect interrupt without reading hardware twice?
	- Option 1: have function that reads whether input level has changed, then read actual input 
	- Option 2: one hw read event, input values get stored locally
- Option 1: hw input values used in processing will not be the same that triggered the input
	- -> go with 2
- Removed max 250ms interrupt timer, keeping just 50ms reset phases

## Timing library:
- Function for hardware debounce?

## Overall thoughts:
- Improved separation of concerns
- Simplified redundant/verbose processes
- Narrowed down definitions/intentions
- Implemented 'camshaft'
- Error handling structures still loose, but getting a better sense of when/where to catch and respond to errors

# 27/02
## README:
- Added notes on engine system dynamics from input -> control output

## Fuel and spark lines module:
- Where to call its update function?
	- Lines are controlled by ECU according to engine states data
	- Most relevant timing component is the 4-stroke cycle
	- -> Lines need to be actuated by 'engine'/'ECU' using timing from valves
- Currently: fuel+spark lines and valve control independently own local stroke states
- Who should own stroke state? What defines stroke state?
- Stroke 'state' currently just index for valve and fuel/spark line arrays
- In this system, the timing update that drives stroke cycle progression is camshaft revolution
	- -> Fuel+spark lines module updated by engine manager, uses engine API to know when cam has completed a new revolution
- Fuel+spark lines updated at the end of engine manager function: module can only use updated engine state data

## Stroke states:
- Do I really need to implement stroke cycle states? Current strokes as array indices system is simple and works
- Keep stroke update only in valve control, make getter via engine api for fuel+spark lines control

## Error handling:
- Removed silent error handling from valves and fuel+spark modules: they just update and assign values, no computing or other operations
