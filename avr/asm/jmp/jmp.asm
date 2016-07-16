.include "../defs/m16def.inc"


.cseg
.org 0
_start:
  jmp main ; jmp to main, _start is located at 0x0


.org 0x090
main:
 ldi r16, 0x0
