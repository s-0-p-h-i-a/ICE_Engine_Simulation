# WIP Folder

This folder contains all the in-progress work on this project. Fully validated modules will be integrated into the Main Project code and folder.

## Current WIPs:

[Main Project v1](./Main_v1_WIP):
* First rework layer, ready for clean-up and then hardware integration
* Semantic clean-up
* In-code comments addition
* Data interfaces for plotter display
* Localised logic refactors
* Goal: v0 was first functional version, v1 will be the first polish

[Crankshaft Addition and Restructuring](./v1_Crank_and_Restructure):
* Offshoot of main v1 refactor to sketch crankshaft integration, new structure, new timing logic
* Experimental/sandbox to explore ideas, may only be fully implemented in v2
* Testing new structure adding an engine layer/module for both the cylinder and the crankshaft
* Crankshaft now moves flywheel, flywheel module is purely responsive to crankshaft + relays position to servo module
* Sketching LED-based crankshaft visualisation
* Implementing centralised timing for LED cylinder and crankshaft
