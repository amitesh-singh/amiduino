# Input Shaping

## Marlin (stock firmware)

- refer to https://marlinfw.org/docs/features/input_shaping.html

Integrated ZV Input Shaper
Marlin first introduced ZV Input Shaping in version 2.1.2. More advanced input shapers may be able to cancel more complex acoustic waves, but even this simple shaper can make a striking improvement in print quality and speed. It’s also so efficient that it can run on AVR, bringing Input Shaping to millions of existing 3D printers with no change of hardware.

This simple shaper introduces a single anti-vibration signal into the stepper motion for the X, Y, and Z axes. It does this at the stepper level by doing normal stepping with 1/2 of the step count at half the frequency, then introducing a second set of step signals that “echo” the first, delayed by 1/2 of the ringing interval. It is a simple and elegant trick, and best of all, it works!

This input shaper is controlled with G-code `M593`. `M593` is supported by ender 3v3 se.

By looking at the stock firmware code, 