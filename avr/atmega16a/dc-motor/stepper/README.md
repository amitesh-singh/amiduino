# Notes about  28BYJ-48 Stepper Motor

- Gear ratio 64:1, some people open the gear box and suggested the value as 63.68395:1
- 15 RPM
- Frequency 100 Hz
- Half step mode is recommended (8 steps control)
- Weight 30g
- Wiring to the ULN2003 controller A(blue), B(pink), C(yellow), D(Orange), E(Red, mid-point)
- Pull in Torque: >34.3 mN.m(120Hz)
- Detent Torque: >34.3 mN.


I have a BYJ48 Stepper Motor and a ULN2003 driver board module.  
You must power this device externally not from MCU circuit.  
This Motor has a Gear ratio of 64 , and Stride Angle 5.625° so this motor has a 4096 Steps.  

steps = Number of steps in One Revolution  * Gear ratio.

This motor has 4 coils and 5 connections to be made if you have the ULN2003 board.  

1. blue    - Coil 1
2. pink    - Coil 2
3. yellow  - Coil 3
4. orange  - Coil 4
5. RED     - it is for Power 5V

## Stepper Methods

### Wave drive

- 4 steps are only unique
- one phase at a time
- simplest but least used
- lowest torque.
- Stride Angle 11.25° hence steps = (360/11.25) * 64 = 2048.

#### Clock Wise

`1000 0100 0010 0001`  REPEAT ~~0001 0010 0100 1000~~

#### Anti Clock Wise

`0001 0010 0100 1000` REPEAT ~~1000 0100 0010 0001~~

### half step

- 8 steps are unique
- one or two phases at a time  
- smallest step angle
- medium torque
- Stride Angle 5.625° hence steps = (360/5.625) * Gear ratio = 64 * 64 = 4096.
- 64 steps required for

#### Clockwise

`1000 1100 0100 0110 0010 0011 0001 1001`

#### anti-clockwise

opposite of above sequence.

### Full step

- 4 steps are only unique.
- two phases at a time.
- Strongest Torque and holding power.
- Stride Angle 11.25° hence steps = (360/11.25) * 64 = 2048.

#### Clockwise
`1100  0110  0011  1001`  Repeat ~~1100 0110 0011 1001~~

## Torque test

`Torque = Force * Distance`  
Say for 30g weight and if i gotta pull it up via this motor using 10 cm shaft, i need to have `30g * 10 cm = 300 g-cm ` - According to some youtube video, this motor has passed the test. conditions were 30g weight and delay 3 ms
Spec says 343 g-cm or `34.3 mN.m`  
Torque = 30/1000.0 kg * 9.8 (or ~10) * 10/100 m = 0.0288 N.m = 0.03 N.m = 30 mN.m
