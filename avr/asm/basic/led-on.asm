
.include "../defs/m16def.inc"

   ldi r16,0b00000001
   out DDRB,r16
   out PORTB,r16

Loop:
   rjmp Loop ; just loop baby
