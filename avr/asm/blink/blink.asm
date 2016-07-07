
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
   ldi r21, 100
   rcall delay
   out PORTB,r17
   ;delay here
   ldi r21, 100
   rcall delay
   rjmp Loop ; just loop baby

delay:
   clr r17
   sec_count:
      cpse r17, r21
      rjmp sec_count
   ret


