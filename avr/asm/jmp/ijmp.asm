.include "../defs/m16def.inc"

.cseg
.org 0
ldi ZL, low(main)
ldi ZH, high(main)
ijmp  ; ZL:ZH => r30:r31

.org 0x90
main:
   rjmp main

