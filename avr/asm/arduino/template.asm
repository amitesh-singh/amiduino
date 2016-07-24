.include "../defs/m16def.inc"

.cseg
.org 0

setup:
; stack initialization
   ldi r16, low(RAMEND)
   out  SPL, r16
   ldi r16, high(RAMEND)
   out SPH, r16
; ------------------------

   rjmp loop

loop:
   ; write your code which gonna run forever
   rjmp loop
