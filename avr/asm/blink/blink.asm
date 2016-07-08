
.include "../defs/m16def.inc"

   ; initialize stack ptr
   ldi r16, LOW(RAMEND)
   out SPL, r16
   ldi r16, HIGH(RAMEND)
   out SPH, r16
   ; end

   ldi r16,0b00000001
   ldi r17,0x00
   out DDRB,r16

Loop:
   out PORTB,r16
   ; delay here
   rcall delay100ms
   out PORTB,r17
   ;delay here
   rcall delay100ms
   rjmp Loop ; just loop baby

delay100ms:
   clr r19
   loopme:
      nop
      rcall delay
      rcall delay
      rcall delay
      rcall delay
      dec r19
      brne loopme
      ret

delay:
   clr r18
   sec_count:
      nop
      nop
      nop
      nop
      nop
      dec r18
      brne sec_count
   ret
