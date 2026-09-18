## ======== DATASHEET ========
### ==== EVENT HANDLER ====
#### Poll rate: ms
- Every 500ms

### ==== ENGINE ====
#### Engine load: %
- Idle load = 20%
- Max load = 100%

#### Input and deceleration rate: 1-10% change in load
- Min/Max throttle input: +1% / +10%
- Min/Max brake input: -1% / -10%
- Deceleration rate: -1% 

Coast phase: 10'000ms

### ==== CRANKSHAFT ====
#### Poll rate: ms
- Angle updated (incremented) every 100ms

#### Angle increment: °
- Converted from engine load % to 0-10 range: load / 10
- Min 2° (idle), max +10°

### ==== CAMSHAFT ====
#### Gear ratio:
- 2:1

#### Intake valve open/close offset: BTDC
- Open: 20°
- Close: 50°

#### Exhaust valve open/close offset: BTDC
- Open: 50°
- Close: 20°

### ==== FUEL + SPARK ====
#### Timing offsets: ° BTDC
- Injection: 20°
- Ignition: 25°

#### Durations:
- Injection duration: 1ms
- Ignition duration: 2ms

#### Duration time amplification (for visualisation): x50
