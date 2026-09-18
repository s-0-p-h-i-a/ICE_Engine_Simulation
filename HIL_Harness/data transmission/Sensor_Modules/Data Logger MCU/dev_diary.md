## 24/01
- Data receive, data unpack, system snapshot build, data display console, data display matrix modules started

---
## 25/01
- Finished 1st draft of data logger MCU code .cpp files

### Structure:
	- data_reception_interface.cpp: receives data, builds received data array
	- data_processing_interface.cpp: processes received data array, builds local data array
	- system_snapshot.cpp: builds system snapshot from local data array
	- data_processing_*.cpp: displays system snapshot on console, displays analog input + colours data on LED matrix
	- main .ino: calls readSystemSnapshot(), displayDataConsole(), displayDataMatrix()
	
### Execution path example (console):
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

---
## 26/01
- Wrote data display and error message parsing + display modules

---	
## 27/01
- Data processing interface previously generated 3 separate array for the bit packed data from each input (pot, joystick, water sensor)
- Now it builds one [9] array and unpack bits func generates uint16 value out of any given 3 elements
- In buildLocalDataArray: j = i * BITPACKED_UNIT_SIZE; indices used inside for loop.
	- i = amount of unpacked data elements to build out of received data array containing data packed in BITPACKED_UNIT_SIZE elements
	- BITPACKED_UNIT_SIZE = amount of elements inside each data packed to be unpacked into one element
- Finished adding header guards
- Compilation: minor include/dependency and syntax errors only, no changes made or required
- COMPILES
- Finished and compiled data logger code
- Started writing data logger mcu documentation
