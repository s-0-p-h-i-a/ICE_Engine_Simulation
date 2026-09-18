# DEVELOPMENT NOTES
## 22/01
- First draft code written for I/O system, basic scaffolding for data interfaces
- First README version done
- Functions index: complete for all first draft modules, no data interfaces yet
- Idea: implement flag for when a value has to be capped to fit valid range, as part of range handling system

---
## 23/01
- Capped value flag implementation:
	- Inside each colour control module:
		- bool cappedToAnalogMaxBLUE, cappedToRGBMaxBLUE variables
		- enum for error codes
		- Add error tracking to main loop
		- Goal: system snapshots of I/O data and of 'error report'
		- Colour control range handling status codes -> error reports interface -> main
	- How to implement this while maintaining encapsulation?
	- Issue: repeated capToAnalogMax and capToRGBMax calls: both inside controlColour and in clampToRGBRange, and at different execution points
		-> centralise range handling inside clampTo... (exception for red: analog range cap for mode assignment)
		-> allows for range handling status codes to represent a specific execution point where an excess value was detected
	- Removed range capping redundancy in favour of range capping error codes belonging to a specific moment -> clearer debugging
	- Red mode selection interface: strictly computes, range handling happens in controlRED
	- Range handling via clampTo.. happening asap after reading hardware input
	- Final implementation: range handling code/state for each colour module, no cappedToMax vars, sticking with overall error code system instead of local individual bool flags
	- 2 tasks happening each main loop: RGB control and error checking/logging
- Data processing for the logger: wrote 16- to 8-bit packer

---
## 24/01
I/O:
- Plotter display module
- Error reports: change functions so a system error report array is generated at every loop
	- checkRangeHandling is now computeSystemStatus
	- No independent variables, all data is stored in a report array
	- generateReportArray now called every loop
	- Execution path: main calls generateArray -> calls computeSystemStatus -> calls getColourStatusElements
	- The array is effectively first filled in the colour elements, then the system code element
	- Goals:
		- No intermediary variables, array is filled directly via getter and compute
		- Data collected/computed as close to each other as possible in execution path
		- Array build process mirrors system error code compute process: colour statuses are computed / colour status array elements assigne, then system status computed using colour statuses / system status array element assigned
	- Added error array to raw data interface
	- Added errors to logger transmission

---	
### 26/01
- General + red system error handling modules rework:
	- Improve layers and concerns separation
	- limittoanalog and limittorgb funcs now static and only called inside clamp func
	- Removed use of limittoanalog in red system
	- Improved separation: red control module does not handle errors anymore, it now only responds to error status computed and handled in error handling RED by not responding and exiting controlred func
	- Mode selection module now: internal validity check + returns joystick system state
	- Further consolidated joystick system error handling execution path:
		- controlRED calls checkJoystickSystemStatusValid() from errorhandlingRED to see whether to proceed
			-> checkJoystickSystemStatusValid() calls static checkIfExcessErrorsBufferActive()
				-> If buffer active: returns 0; checkJoystickSystemStatusValid() returns 0; controlRED() returns and finishes execution
				- If the excess errors buffer is active: redSystemStatus was last updated when handleJoystickInputError detected excess errors and triggered system pause status, redSystemStatus does not need to be updated again until buffer elapsed. checkbufferelapsed will toggle redsystemstatus back to no errors once buffer elapsed
				-> If buffer inactive:
					-> checkJoystickSystemStatusValid checks if input exceeds range
						-> if yes: handleJoystickInputError checks for excess errors, updates redsystemstatus, returns 0
						-> if no: returns 1;
			-> Then, computeJoystickMode() assigns the mode code to internal joystickmode var
			-> processModeSelection(joystickmode, joystickInput) is called
				-> If mode == error: turn LED off
				-> Else: blink or adjust brightness
			-> Exit processModeSelection
		-> Exit controlRED
		
	- Red/joystick system status now updated inside checkbufferelapsed() and handleJoystickInputError(), stays latched until state change triggers new value assignment
	- checkJoystickSystemStatusValid now only calls static handler funcs and returns status, does not manage state directly
	- Replaced redSystemStatus var with joystickSystemStateArray[ERROR_TYPE]
	- Same for errorCounter var with joystickSystemStateArray[ERROR_COUNTER]
	- joystickSystemStateArray[2] has system status in element 1 and error counter in element 2
	- Integrated joystickSystemStateArray data into error reports module so it can be fed to logger
	- Removed error messages from error handling red: i want the i+o mcu to only feed the error codes to logger mcu
	- Goal: have data logger mcu display error messages report with system snapshot
	
- Set scaffolding for error report building process errors: practice thinking about fault paths and fault tracing
- Added inactive/silent error logging as prep towards fault injection
- Modular system design helps with ongoing refactors

---
## 27/01
- Finished and compiled data logger code
- Started writing data logger mcu documentation
- Started updating i/o mcu documentation

---
## 30/01
- Start compile review
- Compile process changes:
	- Data processing (for logger):
		- Data processing module now builds transmission array (previously done inside the transmission module)
		- Transmission module now only calls func from transmission module, does not compute anymore only transmits
	- Data arrays: in error reports, data processing for logger modules, now initialised as pointers to null so i can check whether they have been successfully assigned
	- Using null pointer initialisation, implemented system to check if arrays were assigned before transmitting to data logger
	- Main RGB control func+module calls+owns joystick system error checking: removed as this is already being handled upstream, at the correct layer (controlRED calls checkJoystickSystem)
	- Range handling error system not tight: colour control func calls clamptorgb which updates local error flag, then calls getrangehandling to assign local range handling status var. local error flag from rangehandling module is used repeatedly for each of the modules, this creates a data path for possible undefined behaviour
		- Solution: local array inside range handling module stores status for each type + getter 
	- Observation: range handling module could have its own range handling handling error response system, which could have its own error handling system, etc... -> build error handling so that error handling system is a solid 'error limit'
	- For each colour, place input range handling status update right after clampTo... use to get up to date value
	- Range handling status checking hinges on previous point + clampTo... doing its job, there is still error surface but leaving things as is for now
	- Tradeoff: more #includes + more widely accessible enums, vs less includes and having to redefine locally? For now going with 1st so can have consistent names + not have to redefine + values reflect what they represent
	- Had to make getData function and have main call it so values needed for data processing (plotter+logger) are assigned/updated and available to funcs that need it
- Compile review done!
- Main changes:
	- Improved separation of concerns overall
	- Identified and handled timing/data flow/execution path issues
	- Data now gets generated every loop
	- Started using pointers: to control whether data arrays values were successfully assigned
- Getting better at/more comfortable with understanding and handling compiler errors
