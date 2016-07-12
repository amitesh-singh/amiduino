; specify device
.device ATmega16

.cseg       ; select current segment as code
.org 0      ; begin assembling at address 0

; .set or .equ can be used for constants.
; you could refer to DS for these values
; or use defs/m16def.inc 
.set DDRB = 0x17 ; refer DS   
.set PORTB = 0x18 ; refer DS

   ldi r16, 0xff
   out DDRB, r16

   clr r17               ; clear our counter

loop:
   out PORTB, r17
   inc r17
   rjmp loop
