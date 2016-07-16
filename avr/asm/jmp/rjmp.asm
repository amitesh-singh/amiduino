.include "../defs/m16def.inc"

.cseg
.org 0
jmp main

.org 0x0090
main:
   rjmp main  ; equivalent to rjmp -1
   ; main is at 0x91 and rjmp is at 0x90 so difference
   ; 0x90 - 0x91 = -1
