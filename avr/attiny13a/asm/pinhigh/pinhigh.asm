; This is compiled with 'avra' tool.
; Lets try with mixing asm and C in next example.
; avr-gcc with assember would help.


.include "/usr/share/avra/tn13def.inc"

.cseg
.org 0

_start:
   ldi r16, LOW(RAMEND)
   out spl, r16
   ldi r16, HIGH(RAMEND)
   ;out sph, r16

   ldi r16, 1 ; PB0
   out DDRB, r16

   ldi r16, 1
   out PORTB, r16

   loop:
      rjmp loop
