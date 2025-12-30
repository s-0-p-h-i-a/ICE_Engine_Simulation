# Technical Challenges:

## Servo Powering & Hardware Bring-Up

First time working with a servo motor involved learning processes on both hardware and software levels.

See [Flywheel RPM Repo](https://github.com/s-0-p-h-i-a/Flywheel_RPM_Hall_Sensor) for process and build + test plans.

* The flywheel servo is powered from an external 5 V supply, not the Arduino regulator.

* Custom power and ground wiring was spliced, insulated, strain-relieved, and validated with a multimeter before integration.

* Power delivery was verified independently at the breadboard level prior to connecting the servo.

* Servo behavior was validated incrementally using dedicated test sketches:

  * Fixed-angle sweeps to confirm basic actuation
  * Button and potentiometer control to characterize speed response and angle resolution
  * Boundary testing to observe spill-over and limit behavior

* Observed servo speed characteristics were cross-checked against datasheet values to derive safe angular step sizes for deterministic RPM generation.

* The Hall sensor and RPM timing logic were introduced only after stable actuation was confirmed, allowing electrical, mechanical, and firmware issues to be isolated during bring-up.

---
## Software & Firmware Challenges

The project exposed several non-trivial software challenges that were addressed in v0:

* **Cylinder display state initialization**

  * Original state matrix stored junk values because it was not declared as the right type.
  * Debugging revealed interactions between arrays and pointers inherited from the previous C-based implementation.
  * Additional hardware-level issue: one pin was floating, contributing to inconsistent state display.

* **Cylinder display timing and update synchronization**

  * States jittered due to function calls updating the cylinder stroke at different times and levels.
  * Sending the stroke state variable to the plotter revealed timing overlaps affecting visual consistency.
  * Resolved by restructuring updates and aligning state propagation across functions to ensure deterministic behavior.

* **Servo control and edge handling**

  * Initial logic caused two failure modes:

    1. Servo swept from 0° → 180°, then gradually to a smaller radius before jittering around 90°
    2. Servo moved to 180° then jittered at that limit
    
  * Root causes:

    * Edge-value handling issues
    * Incorrect instruction order in sweep logic
    * Step increment/decrement mistakenly declared as unsigned, causing wrap-around
    
  * Firmware was reworked to handle edge cases correctly and ensure smooth, predictable actuation.
