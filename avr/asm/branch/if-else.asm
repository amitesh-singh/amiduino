.include "../defs/m16def.inc"

.cseg
.org 0


ldi r16, 0
_start:
; how to write if else statemeents in asm
   cpi r16, 0
   brne PC + 2
   rjmp isZero

   cpi r16, 1
   brne PC + 2
   rjmp  isOne

   cpi r16, 2
   brne end
   breq isTwo

isZero:
; ldi r16, 0
   rjmp end
   isOne:
;ldi r16, 1
      rjmp end
   isTwo:
;ldi r16, 2
      rjmp end

   end:
      nop
      nop
      ldi r17, 1
      add  r16, r17
      cpi r16, 3
      breq reset
      rjmp _start

   reset:
      ldi r16, 1
      rjmp _start
