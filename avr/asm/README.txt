install avra
------------
- sudo apt-get install avra

Most of programs are for atmega16

how to compile
--------------
- avra blink.asm
links
------

- https://sites.google.com/site/avrasmintro/
Notes
-----
 r16-31 are general purpose registers for programmers. they are easy to use
 e.g.
 ldi r16, 0b01110001

 - For r0 - r15,
 ldi r16, 0b00000101
 mov r2, r16

avra quick tips
----------------

To generate map file: avra -m file.map file.asm
To generate list file: avra -l file.list file.asm


We can use atmel avrasm2.exe (located in ./bin) too

good avr asm simualator for windows
-----------------------------------

http://www.amctools.com/atmel.htm
