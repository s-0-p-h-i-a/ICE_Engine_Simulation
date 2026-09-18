# DEVELOPMENT NOTES

System integration process notes.

See I/O MCU dev diary and Data Logger MCU dev diary for respective process logs.

---
## 02/02

### UART COMMUNICATION TESTING
### Write tests using code from I/O MCU:

- Test 0: send + receive just 1 const int, only console display, no matrix
- Test 1: send + receive const array of 5 1's
- Test 2: send constant 1 for all data array elements, test with other constants, only console display
		- Bit-packed elements: multiplier = rest = flag = 1 -> final result = 256
		
- Test 3: bit-packed low and flag = 0, other elements get incremented by 1 at every transmission event
- Test 4: bit-packed low and flag = 0, other elements: random numbers <= 255 (using srand and time)
- Test 5: pack numbers > 255, other elements constant
- Test 6 (final): random numbers <= 1023 for bit-packing elements, random <= for the rest

### Testing process:
- Test 1: both MCUs compiled + uploaded, but communication did not work
	- Solution: Test 0 : send + receive just 1 const int, only console display, no matrix
	
- Test 0:
	- First run: 
		- System snapshot counter works
		- Transmission pauses not working
		- Transmission int value is junk (10, 49, 13) (ascii values? yes: without adding DEC to serial print, monitor prints ascii value of character)
	- Changes: check code, validate transmitter mcu behaviour on serial monitor, then connect with receiver mcu again
	- Fixed: time elapsed check was previously delta > 0 instead of delta > TRANSMISSION_PAUSE (+fixed in main code)
	- Second run:
		- Snapshot counter + timer now working, still seeing ascii value numbers

---
## 04/02
### Test 0 debugging continued
- 3 ideas:
	1. Serial monitor display file did not include stdint -> getreceivedinteger returned uint8, serial.print interpreted it as char?
	2. Try just sending + receiving + displaying data as char?
	3. Missing library includes: if so, use stddef or cstddef?
- Try 1? Actually, display interface still displayed snapshot counter properly even without stdint include -> probably not the solution
- Try 3:
	- Include stddef first: arduino is c++, but it compiled stddef, so it probably recognises it
		- Did not work
		- Pretty sure including stddef was what fixed size_t not being recognised as a type by the compiler, which supports arduino cli recognising and including it properly
	- Include <cstddef> instead just to see if anything changes
		- Arduino cli does not compile it, change back to stddef
		- Removed stddef, turns out it was not needed for byte type to compile
- Try 2: only use char
	- Result: 
		- 1st attempt: serial monitor displays �, nothing, or 1
		- 2nd attempt: serial monitor displays �, gets stuck at 5 snapshots.
	- Given previous varying between the same 3 different outputs, maybe it's serial getting 'clogged' and needing to be cleared? It did display 1 a couple times in the 1st attempt
	- Tried a 3rd time: repeating display pattern:
	
		System snapshot number:1 Received data: 1 
		System snapshot number:2 Received data:
		System snapshot number:3 Received data:
		 (5s pause)
		System snapshot number:4 Received data: 1 
		System snapshot number:5 Received data:
		System snapshot number:6 Received data: 
		
		- When highlighting a line, an empty space is visible on the lines that display nothing
	- Try serial.flush: tried twice, did not change anything, keeping it for now
	- Adding built in led blink check to sender mcu for visual check
	- Other idea: potential issue with timing check in sender mcu, or in read/receive logic in receiver mcu
	- RX LED on transmitter MCU and TX LED on receiver MCU blink every 5s ??
		- Try different wiring:
			- Connected only receiver RX to transmitter TX: only receiver TX led occasionally blinks, serial monitor displays junk values
				-> Issue with receiver mcu logic likely
		- Added receiver TX and transmitter RX wire:
			- Back to previous output + blink behaviour
		- Connected only receiver TX to transmitter RX:
			- Nothing on monitor, transmitter RX led occasionally blinks
			
- Issue found: using serial.println sends ascii values, should have been using serial write
	- But pretty sure I tried println with DEC and it still didnt work?
- Moving on: keeping char only data transmission+reception+display for now, now checking send/receive logic on both mcus
	- Groups of 3 snapshots showing up, 5s pause in between them
		-> Transmit logic could be sound, and it is sending 1 but somehow receiver is receiving 1+2xnothing
	- Try serial.available == 1 in receiver
		- Still same output
	- Serial display func called every loop in receiver mcu + loop cycle faster than serial cycle -> a couple extra loops run in the time it takes for serial comm to 'close' between transmitter mcu sending new data
	- (Still does not explain why receiver TX and transmitter RX leds blink??)
- Idea: receivedata and displaydata funcs are called one after the other in main loop
	-> Timing issues, bool getnewdatareceived func returning true causing displaydata func to get called?
	-> Maybe bool newdatareceived staying true for too long?
	- Move receivedata inside displaydata: did not change anything
	- Make receivedata bool type:
	main loop:
		if (receiveData()) {
	
		displayDataConsole();
	}
	- Even if this specifically isnt the issue, better syntax than with the previous bool var intermediary
- Removing serial flush as it wasnt needed in the initial attempts, only one message per 5s was showing up
- Also going back to serial.available > 0 as changing that also did not solve things
- Trying swapping serial.println for .write: now it works!! It was about .println vs .write all along
- Conclusion: sending char seems to keep the serial buffer open for longer?

### Moving on to test 1
- Test 1 (const array[5] of 1's): succeeded!!

---
## 06/02
### Test 2:
- Produced no output at first
#### TX side:
- Removed checkIfArrayNotGenerated() call to focus on just TX->RX
- Wrote sendDataToPlotter func and added static snapshot counter var in i+o module to view the data getting transmitted
- Used console display func from logger mcu as template

- Plotter display result: constant transmission of array full of 0's and snapshot counter stuck at 0
	- This means:
		1. Local array gets initialised at 0 = data processing module is not packing it properly
		-> this is why there was no output, checkIfArrayNotGenerated was preventing the mcu from transmitting anything
		2. Transmission pause logic isn't working

- Transmission pause fixed: forgot to add delta elapsed function call in plotter display func
- Result:
	- Pause now fixed, still displaying all 0's
	- 0's: forgot to increment snapshot counter
	- Array build:forgot buildTransmissionArray call

- Added increment and buildTransmissionArray call
- Result:
	- Snapshot counter stuck at 1, array = { 1, 1, 1, 0, 0, 0, 0 }
	- Constant transmission, no pause
	- Issue: condition check before transmission
	-> forgot to swap a || for && while writing

- Changed transmission if check
- Result:
	- Same numbers as before
	- Transmission every 5s
	-> bit packing logic is working
	-> issues with local array build and snapshot incrementing remains
	
- Forgot generatelocalarray call in buildtransmissionarray -> fixed
- Removing checkIfArrayNotGenerated call wasn't doing anything as generatelocalarray wasn't being called anyway
- Initialising local array at 0 helps me find what part of the process is failing
- buildtransmissionarray check in display func only fully works if buildtransmissionarray runs and checks generatelocalarray
- Using bool instead of void type for funcs helps me find errors
- Added generateLocalArray call, now if there is an issue w the build process in data interface layer there would be no plotter display
- Result:
	- No plotter display -> generateLocalArray is not working as it is the only thing that would make buildTransmissionArray return 0

- checkIfArrayNotGenerated is the only thing that can make generateLocalArray return 0
	-> look for logic issue here
- Plotter display has shown that the bitpacking part of the transmission array build process is working
	-> 1st local array element assignment OK
- Check for other elements check inside checkIfArrayNotGenerated
- First rename to checkIfArrayGenerated as previous name was confusing, implementation required thinking in double negative
- No other changes inside this func, change call in generateLocalArray from checkIfArrayNotGenerated to !checkIfArrayGenerated
- Result:
	- Correct array displayed every 5s
	- Snapshot counter still not incrementing

- How is snapshotcounter not incrementing when checkIfPauseElapsed is otherwise doing its job?
- Considered changing snapshot incrementing to display func, but this state update belongs in the timing check
- Added bool pauseElapsed inside checkIfPauseElapsed to be returned, instead of 2 direct returns
- Result: still did not work

- Moved ++snapshotCounter; to inside display func
- Result: now stuck at 2

- Changed checkIfPauseElapsed back to 2 returns bc visually clearer and it didn't change anything
- Moved snapshotcounter to data processing layer, now is incremented everytime buildTransmissionArray runs and plotter display uses getter to access it
- Result:
	- Had broken something in timing check, constant stream of snapshot at 95
	- Fixed timing check, still at 95

- Disconnected then reconnected MCU
- Result:
	- Stuck at 2

- Reverted incrementation back inside timing check, declared counter at 1 and did snapshotCounter = snapshotCounter*2;
- Result:
	- This works
	
- Changed to snapshotCounter++
- Result: works if initialised at 1, gets stuck at 1 if initialised at 0

- ++snapshotCounter works if initialised at 1

- if (snapshotCounter == 0) {
			snapshotCounter = 1;
		}
		++snapshotCounter;
	-> gets stuck at 1
	
- Choosing to leave this for now as snapshot logic in main project is in receiver mcu and it works there
- Moving on to integration with rx mcu

#### RX Side:
- Testing with rx mcu: nothing displayed
- Start checking at data reception and display layers to check for errors, then follow execution paths in the middle layers
- Display func was not calling buildLocalArray
- buildLocalArray is now the only thing that can block displayDataConsole execution and it always returns 1
- Result: still no display

- Initialise local array at 0 in case it's a data processing issue
- Result:
	- Moving GND wire in and out of pin causes a batch of snapshots to be displayed
	- First element always 0, rest is junk value
	
- Junk value arrays after initialisation at 0: build process isnt working
- Found an issue:
	localArray[FIRST_ARRAY_POSITION] = unpackData(bitpackedArray);
	-> was previously using the wrong index
- Result: no display

- TX/RX lights aren't blinking

- Removed else for return 0 in receiveArray


	

