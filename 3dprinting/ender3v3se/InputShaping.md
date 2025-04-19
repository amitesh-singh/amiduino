# Input Shaping

## Marlin (stock firmware)

- refer to https://marlinfw.org/docs/features/input_shaping.html

Integrated ZV Input Shaper
Marlin first introduced ZV Input Shaping in version 2.1.2. More advanced input shapers may be able to cancel more complex acoustic waves, but even this simple shaper can make a striking improvement in print quality and speed. It’s also so efficient that it can run on AVR, bringing Input Shaping to millions of existing 3D printers with no change of hardware.

This simple shaper introduces a single anti-vibration signal into the stepper motion for the X, Y, and Z axes. It does this at the stepper level by doing normal stepping with 1/2 of the step count at half the frequency, then introducing a second set of step signals that “echo” the first, delayed by 1/2 of the ringing interval. It is a simple and elegant trick, and best of all, it works!

This input shaper is controlled with G-code `M593`. `M593` is supported by ender 3v3 se.

```
Send: M593
Recv: Input Shaping:
Recv:   M593 X F42.00 D0.15
Recv:   M593 Y F40.00 D0.15

```
Here D0.15 is the `damping factor`


By looking at the stock firmware code, `Configuration_adv.h` these are the defaults values set in the firmware.

```
#if ENABLED(HIGH_SPEED)
    #define INPUT_SHAPING_X
    #define INPUT_SHAPING_Y
  #if EITHER(INPUT_SHAPING_X, INPUT_SHAPING_Y)
    #if ENABLED(INPUT_SHAPING_X)
      #define SHAPING_FREQ_X  42//34.29  // (Hz) The default dominant resonant frequency on the X axis.
      #define SHAPING_ZETA_X  0.15f     // Damping ratio of the X axis (range: 0.0 = no damping to 1.0 = critical damping).
    #endif
    #if ENABLED(INPUT_SHAPING_Y)
      #define SHAPING_FREQ_Y  40//41.38    // (Hz) The default dominant resonant frequency on the Y axis.
      #define SHAPING_ZETA_Y  0.15f    // Damping ratio of the Y axis (range: 0.0 = no damping to 1.0 = critical damping).
    #endif
    
```

## how to set Frequency in both X and Y using octoprint

the below commands change the frequencies for X and Y respectively. This does not change the Damping factor.
```
M493 X F42
M493 Y F40
```

To change `Damping factor`

```
M493 X F42
M493 Y F40
```

## Disable input shaping

If you want to temporarily disable Input Shaping without reflashing firmware:

```
M593 X F0
M593 Y F0
```

## Calibration in marlin 

### single layer method