how to make attiny85 to use 16 MHZ internal RC oscillator.
----------------------------------------------------------

avrdude -c usbasp -p attiny85 -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m
