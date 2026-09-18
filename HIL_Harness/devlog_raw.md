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

# 22/04
Main goal: review structure, dependencies and main execution paths. Change/refactor as needed
Initial criterion: what would make it easier for me to review the code AND would be better design?

Starting point: move from main.c inwards (main -> sysmanager + interruptmanager -> crank etc)

- Main 
- Event handler 
- System manager

## Main:
- Removed error handling -> to be done inside system manager
- Conceptual change:
	- Previous idea: main initialises individual modules separately, detects and responds to execution errors inside them, acts on two different layers (hardware and sysmanager)
	- Now: main is a dumb wrapper, it knows nothing, only includes sysmanager and acts as a 'driver' for the program, only initialises system and runs it
- Putting system init check back in main so it can prevent further execution + sysmanager doesnt need to check init status each loop?
- NO: I still want to be able to know where the initialisation failed + send this data, and i had decided that main will not handle errors
- But in this case while loop would keep running even if init failed? -> if branch

## Error handling:
- Previously contained MainStatuses enum (used in error handling inside main)
- Moved to system manager for now, considering whether to keep it or separate and localise it
- System manager or init error desired execution path: skip straight to creating+packing+sending data log, exit while loop
- Engine manager error -> fully end execution or just skip to next sysmanager loop?
- For initial testing and debugging: fully exit so the data from the loop where the error happened is the last to be sent

## System manager:
- Reviewing enums in system manager: do the structure and responsibilities make sense?
	- Start by reviewing function definitions/calls
	- What do I want to do and how do I want to do it? -> define error handling/status tracking
	- Do I want error statuses or states? Error status -> only present if error, state: always defined
- SystemManagerStates -> InterruptStates? Does having these states actually do anything? It has no impact on ISR or on error handling
- Interrupt state represents if an interrupt has been detected this loop -> update in sysmanager or interruptmanager would refer to the samer loop
- setsystemstatus not needed -> system status/state internal to sysmanager layer
- Separating system manager execution statuses (to store eventual execution error points) vs general system state (error/no error)
- How to handle system init failure and prevent manager execution in main if init fails?
- Goal of error handling inside sysmanager: track when+where errors happened
- More specific error handling (eg compute errors) to happen inside individual modules
- Define systemstatus before or after systeminit check? Before so state is never undefined and stays updated
- Move systeminit back to main? Yes -> systeminit check not needed/relevant in manager anymore
- Logic: manager only runs IF initialisation ok
- Repeated systemstatus = init error assignments in init function, is there a way to remove that without losing logic structure?
- What is the real goal? Do I want to prevent execution if init fails, or to know if hardware or engine init or both failed? I want the info
- How to get + send error data AND prevent further execution if init fails?
- -> sysmanager bool to exit while loop
- Systemstatus = ok in runsystemmanager? If init ok, systemstatus will already have been set
- How to avoid repeated sendData calls in sysmanager func?
- Error statuses = last part of the execution process where an error ocurred. Data transmission status assignment would overwrite engine manager error status: ok because engine manager will have its own local error codes, same for crank, valves, etc modules -> system status can be used to debug


## Interrupt/event:
- Maybe move polling to system manager?
- -> "interrupt" now only detects hardware input? Might be cleaner/more debuggable
- Integrate polling into engine manager?
- Introduce states/error handling in this layer?
- What would be an interrupt 'error'? Issue with hardware input belongs in hardware + hardware interface layers
- Interrupt layer doesn't need it if it has no error surface
- Is interrupt timer needed if i remove the polling?
- Change interrupt system to event handling system
- Keep polling, switch to delta instead of modulo. How to prevent overflow?
- How to rework event handler? Feels like it could be smaller/cleaner. -> Start in main func (eventDetected)
- Remember HAL ticks != loop ticks -> systicks could be at 49 one loop and 53 the next -> checking for systicks == PHASE not good
- Collapsed event handler funcs into just bool eventdetected and bool listennow. Defined event handler states
to store current loop state for data log
- Error handling in event handler? Is there enough error surface? No error handling there for now

## Engine manager:
- Integrate former crank ticks + use same timing for input polling (previously in interrupt manager)? No
- Centralised engine timer? Engine layer is central representation of the simulated physical system, centralised timing would be coherent
- How to keep °/ticks unit used in crank? Feed timing data to crank layer in updatecrank call?

## Crank and piston module:
- Move ticks to engine manager?

## Documentation:
- Added condensed project summary + module role to main
- Added module description in event handler

## General notes:
- Statuses vs states? Decision: statuses = error statuses, states = relate to main process
- Tradeoff: preventing further execution + repeated code vs fully executing function + no repetitions vs extra if branch layers?


## TO-DO

## IDEAS
- Main: repeat systeminit attempts if it fails at first


# 23/04
- Continue review/rewrite process started yesterday

## Engine manager:
- Added engine system status update in reset func
- Removed redundant returns in update state switch branch
- Added engine undefined case in update func
- How to handle undefined state? What should happen in next loop? How to handle potential transitions from other states to undefined?
- Issue found: engineON value defined but never updated -> added to init, reset, other relevant points
- Engine init -> sets engine to idle?
- Do i want engine to be on by default at runtime? Or have user turn on/off via button? -> button
- Passing newinputthisloop as argument instead of letting engine manager get it from other module
- Engine undefined <=> engine off?
- Where to update engineON? Engine on/off toggle = response to button input
- Handle button input in engine_input state? Different situation as throttle/brake input
- Add engine toggle state? Yes
- Toggle overrides other actions and impacts all states -> handle invidivually before switch branch?
- Add new button input getter. Where would it come from? Needs to interact with hw interface, but does it make sense to put in throttle/brake module? Somehow yes: on/off toggle = engine load variation. But handled separately as engine on/off toggle overrides things like calculating new engine load
- Error handling in updatestate? No error surface for now
- Update state: early return if engine off or let case undefined handle it? Case undefined -> explicit handling of ALL situations inside switch branch
- Finish routine: now resets to idle if status not ok AND engine on (engine off state/status updates are handled by handletoggle
- Is default state relevant in update state? With undefined now included, all possible states are covered
- If engine is not in any of these states -> error -> return 0
- Changing engine undefined to engine off state, handling with early return 1. Keep undefined state as backup? Not for now
- Bool changestatenextloop not used -> removed
- Created engine manager helpers module for better readability inside main manager
- Engine manager module previously doing a lot on its own, now main module only updates, manages state and has runenginemanager func. Helpers module contains helper funcs (under helpers header -> for main manager only) and getters (under getters header -> used in central 'engine api' module for data transmission)
- Catch invalid throttle and brake input at read before computing new overall input
- Do i want to block checking for new input if load is already at min or max?
- Added computenewload + fixed uint/int compatibility issue

## System manager:
- Added intro comment
- Issues:
	1. At first loop, event may be detected before engine system has initialised -> undefined behaviour
	2. Checking systemStatus != INIT_ERROR value happens at every loop, when init fail should fully end execution -> if it is true once it will stay so until program is terminated. In this case I would not want a data long to be created and sent either
- 1 -> not an issue as systeminit calls engine init, so by the time sysmanager runs, engine system will have a state
- 2 -> changed if check to == init error and added early return 0
- Removed redundant returnvalue initialisation -> declared + assigned right before it is actually needed

## Throttle / brake / ignition:
- Added shell for checkEngineToggle
- Typedefs for more readability
- Changing around uint8 vs uint16 for input processing math

## Crank and piston module:
- Timing system: keep internal ticks or use HAL ticks? Loop ticks will have different individual durations based on what happens in each loop -> not accurate physics
- Reuse listennow from event handler as base for new timing system
- Angular speed: ° per 10 loop ticks -> ° per ~10ms
- currentEngineLoad > ZERO_LOAD relevant? crank doesnt get called if engine is off, if engine on min load is always idle load
- Instead of being passed as argument for update system ticks then reassigned, ticker var updated inside update func
- Add similar 'polling' as event handler: if 10ms phase not elapsed, do nothing
- Clamp and correct crank angle at update, not at cam func call as arg -> both crank and cam working off the same cleaned angle value
- Removing other calls by argument to use local vars -> more readable, less data going back and forth
- Changed piston update from separate ifs to if else branch to reflect mutually exclusive states
- Deleted leftover helper funcs and function defs from previous intermediary work
- Added crank system status update at end of manager func if all is ok
- Added similar 'last error point' status similar to system and engine managers -> similar debugging data meaning across system
- Crank system status update now in main manager instead of push piston
- Keep crank angle update inside push piston (imitating real life piston applying force to crank) or move to manager (better programming wise)?
- Went for crank angle update then piston: piston update updates its state, which is defined by crank angle
- Update piston logic looking fragile/wrong, changing to more explicit implementation
- Defining angle ranges for piston TDC / BDC

## Camshaft:
- Removed % 2 angle update condition
- How to implement angle ranges for valves on/off? Open/close if angle ranges are passed?
- Look up example angle ranges, use rounded values
- Continuously update valves or only toggle when angle moves in/out of ranges? For now only toggle
- Stroke status is needed for this -> move stroke state to cam: cam revs <=> stroke cycle phase change
- Intake valve opens during (at the end of) exhaust stroke, exhaust valve opens at combustion stroke
- Actually there is a valve transition happening at every stroke
- Switch branch to handle the valve transitions? Too verbose and repetitive -> threshold angles array
- Advance in stroke phase and valve toggle are two different types of events


## HW interface:
- Add note to clamp to analog max at read
- Add note to add button read + debounce

## Ideas:
- Include warning if load goes above max?

## General notes:
- 'Cascading' error codes system: engine manager can know if cam update error -> cam manager can know if valve update error -> valve module can specify which valve

# 24/04

## Fuel and spark lines module:
- Rework -> more specific timing, currently just on or off for entire strokes
- Look up fuel and spark timing info:
	- How is timing measured, in ° or ms?
	- Measured in crank ° BTDC or ATDC
	- Injection soon after 270°, spark soon before TDC
- Ignition: "typically 15-35 degrees before TDC (top dead center) of the power stroke"
- Injection: " typically ranges between 10 and 30 degrees before top dead center (BTDC)"
- Going with ignition angle = 25 BTDC, injection angle = 20 BTDC
- Haven't looked into whether this is coherent w the current valve angles, using current numbers for simplicity sake
- Defining TDC offset consts to calculate timing
- Setting timing 'trigger' as detecting that the timing angle has been passed (introduces loop length timing latency, for now mitigated by poll times for event detection and crank angle advancement)
- How to turn injectors/spark plugs on/off? Ideal behaviour: on at trigger detection loop, off in all other loops? Or on then back off in same loop? Look up how the real life events happen
- Spark duration: numbers range from 0.6 to 3ms, depends on factors like cold vs warm engine, pressure etc
- Injection duration: found graph with ~1ms, depends on fuel mass, also depends on the factors above
- Assuming spark duration = 2ms, injection = 1ms: shorter than 1 loop, use timer inside update function
- Update func: checks if current stroke is compression (-> spark) or exhaust (-> fuel)
- Would using systicks and checking if 1 or 2 ms elapsed take longer than setting state to on, then a couple empty instructions, then back off? Implementing time check for 'realism'
- Time passed between crank angle reaching timing angle and execution path reaching fuel+spark lines update already introduces delay? Advance update point? Not for now but to keep in mind for later
- How to do this module without repeating similar commands twice? Using functions helps a bit but seems to just shift the repetition to elsewhere. Use arrays? Main issue: turning on/off (true/false) the actual local vars for fuel and spark state
- Array would have mixed data types. Struct? Have no used them before, time to learn
- Can a struct have const elements (angle and duration) and just one changeable variable? yes
- Haven't worked with pointers much, needed for function calls and access to struct elements using ->
- The while loop + HAL gettick call might take longer than 1ms, again abstracting this for now
- Implementing multiplier to make on/off possible to be seen by eye when using monitor to track engine run data
- Forgot to write constants in all caps, fixing this where i see it
- While loop currently blocks rest of execution: to avoid introducing run time irregularity + given that open/close ranges are extended for visualisation, use if check instead
- Awkward implementation, still involves repetition and verbose state checking -> switch to FSM based management
- Missing init function -> add
- Feel like module could be condensed/streamlined more but leaving as is for now

## Camshaft module:
- Add init func

## Valves:
- Add init func

## Notes/ideas:
- Add init to cam, valves and fuel+spark: init ok check chain from sysmanager down to all engine sub modules
- Spark and fuel timing latency/drift due to loop run time: fix then later use for fault injection

## TO DO:
- Consts -> all caps

# 25/04
## Crank and piston module:
- Found issue: was incrementing total engine load to angle instead of the angular speed this load represents
- Add conversion ratio + clamp increment func
- Piston only TDC or BDC, introducing in between states
- Now piston state managed by switch branch

# 26/04
## Engine manager:
- Noticed engine manager was missing coast timer: coastTimeElapsed was being called but hadnt been written
- Added startCoastTimer to call when state transitions to coast
- Coast phase = 10s

# ??/05
## Documentation:
- README, structured dev log, known issues+abstractions+to do, datasheet all written + up to date

# 12/05
## STM32 integration:
- Created startup/HAL files on cubemx
- Looked through stm32 documentation and datasheets to identify processor version of my mcu, look at pin map, pins table and select which pins to use for analog+digital inputs
- Potentiometer: PA1, joystick: PA2, joystick button: PB12
- Skimmed through code to get overview of start/init process, drivers, dependencies, structure
- Searched for initialisation code for the selected analog+digital pins
- Integrated project files into cubemx generated project folders structure
- Started integrating STM32 specific code in hardware interface
- Which HAL files do I need to #include in my own project files?

# 17/05
## STM32 integration:
### Hardware interface
- Look up: how to use HAL ADC get value for analog input?
- Write ADC channel config and ADC read value funcs
- Which files to include? only HAL .h needed
- Converted old main to simulator main with bool simulator_main: contains while 1 loop, function call replaces while 1 loop in cubemx generated main
