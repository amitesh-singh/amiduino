how to overclock attiny85
--------------------------
1. connect external clock feeding 30 MHz to XTAL1 pin (PB3)
2. how to run LTC1799 at 30 MHz

  f = 10 * 1000000 * 10 * 1000 / (N * R)

   N = 1
   R = 3.25K

   = 10 * 1000000 * 10 * 1000 / (1 * 3.25 * 1000)
   = 30769230.769230768 Hz
3. while compiling binary,mention F_CPU = 30769230

4. I have tested with simple blink program, it seems to works well.
   100ms delay - 0.1002 s shown on Saleae LA (24 MHZ sample rate)
   1 s delay - 1.002 s shown on LA

5. Fuse settings for attiny85.
-U lfuse:w:0xe0:m -U hfuse:w:0xdc:m -U efuse:w:0xff:m 
I enabled Brown out detection at 4.3 V. did not try it with brownout though
at 30 MHZ
