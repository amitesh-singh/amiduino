.include "../defs/m16def.inc"

; definitions
.DEF count = r16
.DEF temp = r17

.org 0x0000

_start:
   ldi temp, 0xff
   out DDRB, temp ; make all ports OUTPUT

   ldi count, 0x00 ; set count to 0

loop:
   out PORTB, count ; output count to PORTB
   inc count ; increase count by 1
   rjmp loop
