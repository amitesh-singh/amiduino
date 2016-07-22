.include "../defs/m16def.inc"

.cseg
.org 0

_start:
   rjmp main

main:
   sbi DDRB, 1 ; set PB1 as output
   ;sbi PORTB, 1 ; set PB1 high

loop:
   PB0ISPRESSED:
      sbis PORTB, 1 << PB0
      rjmp PB0ISPRESSED

      ; LED jalaao
      sbi PORTB, 1 << PB1

   PBOISRELEASED:
      sbic PORTB, 1 << PB0
      rjmp PBOISRELEASED

   cbi PORTB, 1 << PB1

   rjmp loop
