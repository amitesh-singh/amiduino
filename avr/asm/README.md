install avra
------------
- sudo apt-get install avra

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

