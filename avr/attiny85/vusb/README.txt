Fuse settings
=============
avrdude -c usbasp -p attiny85 -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m

This is required to run attiny85 on 16.5 MHz (internal PLL clock)
with the use of ext. crystal.
