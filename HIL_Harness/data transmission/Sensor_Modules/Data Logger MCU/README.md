# ANALOG I/O + DATA LOGGING PROJECT

This project consists of a system with 2 MCUS and I/O peripherals connected via UART:
	- A control MCU that creates output based on user input and periodically sends data to the data logger MCU
	- Data logger MCU receives control system snapshots and processes it for visual display

---
# Data logger MCU

This MCU periodically receives 8-bit system data arrays, processes it into a local array and displays the data on the console and on an 8x8 LED matrix.

It receives I/O values and system status reports.

Analog input values from the I/O control MCU are packed into 3 element

##Folder Structure & System Status:
- data_logger folder: data logger MCU first draft code, compiled but not verified
- test folder: test scripts for progressive testing, fixing and reintegration of first draft code

Current test in progress:
	- Test 2:
		- Transmitter MCU (I/O) sends a constant array[7] of 1's to the receiver MCU (data logger).
		- The receiver MCU then processes it into a local 5 element array.

- The first 3 elements of the transmitted array represent a bit-packed uint16 integer x = 256:
	- 1st element: x / 255 = 1
	- 2nd element: x % 255 = 1
	- 3rd element: boolean flag indicating whether uint16 x had to be bit-packed for uint8 representation
		-> if yes: = 1
		
The data logger output is then: { 256, 1, 1, 1, 1 }

This test excludes all other mechanisms in the I/O + data logging system project and simply focuses on validating UART data transmission.

---
# SYSTEM
## INPUT
- System data from I/O MCU

## OUTPUT
- Prints all transmitted data, formatted, onto console
- Writes data snapshots onto text file
- Matrix displays input and colour control value levels, clamped to [0,8]

---
## STRUCTURE
### Main program
- Reads input from serial, builds 'raw' input data array
- Processes data -> local data array
- Console output: prints data to console
- Matrix output: shows I/O data on LED matrix

### Data reception
- Analog uint16 values from input sources arrives bit-packed into 3 element uint8 array chunks:
	- input value / 255 (integer division)
	- input value % 255
	- flag: true if input value > 255
- Received data array:
	- 3x 3 element bit-packed data arrays: analog input values
	- 4x colour control values: brightness and red colour blink cycle
	- 2x joystick system status: error status code, error counter
	- 4x system data: whole system status code, 3x colour control status codes

### Data Output
- Console:
	- Local data array + reference files -> 2 display matrices: one for titles, one with data
- Matrix:
	- Shows I/O data: 3x input values, 1x RGB LED system control values, 3x individual LED leg control values)
	- I/O data clamped to [0, 8], variables are each assigned one column
	- Level 0: lights off, level 8: whole column on 

### Helper files
- Received data processing: builds local array
- Display data arrays: builds title + data arrays for console display
- Console display and error codes interfaces: libraries of strings to build titles array

!! NOT UPDATED !!
### Dependencies
- Structure:
	- data_reception_interface.cpp: receives data, builds received data array
	- data_processing_interface.cpp: processes received data array, builds local data array
	- system_snapshot.cpp: access interface for local data array
	- Console display path:
		- display_data_arrays.cpp builds 2 display arrays: char array for titles and error messages, uint array for data
			- Uses console display and error reports interfaces to build output array
		- console_display.cpp prints system snapshots to console
	- main .ino: calls readSystemSnapshot(), displayDataConsole(), displayDataMatrix()


### Execution path example (console)
- main .ino: main loop() calls readSystemSnapshot() from system_snapshot.cpp
	- -> system_snapshot.cpp: readSystemSnapshot() calls buildLocalArray() from data_processing_interface.cpp
		- -> data_processing_interface.cpp: buildLocalArray() calls receiveDataArray() from data_reception_interface.cpp
			- -> data_reception_interface.cpp: receiveDataArray() reads serial input, builds receivedDataArray
			- -> data_reception_interface.cpp: receiveDataArray() finishes execution
		- -> data_processing_interface.cpp: buildLocalArray() calls getReceivedDataArray() and getReceivedSnapshotCounter()
			- -> data_reception_interface.cpp: getReceivedDataArray() and getReceivedSnapshotCounter() return the values
			- -> data_reception_interface.cpp: getReceivedDataArray() and getReceivedSnapshotCounter() finish execution
		- -> data_processing_interface.cpp: buildLocalArray() builds localDataArray, finishes execution
	- -> system_snapshot.cpp: readSystemSnapshot() finishes execution
	
- - main .ino: main loop calls displayDataConsole() from data_display_CONSOLE.cpp
	- -> data_display_CONSOLE.cpp: displayDataConsole() calls getSystemSnapshotData() from system_snapshot.cpp
		- -> system_snapshot.cpp: getSystemSnapshotData() calls getLocalArray() from data_processing_interface.cpp
			- -> data_processing_interface.cpp: returns localDataArray element, finishes execution
		- -> system_snapshot.cpp: getSystemSnapshotData() finishes execution
	- -> data_display_CONSOLE.cpp: displayDataConsole() finishes execution
- back to main .ino, calls displayDataMatrix, same path
