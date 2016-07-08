; note: this command does not do much on avr. its only for quick demo/brush ups

.include "../defs/m16def.inc"

_start:
   ; set up our stack ptr so that calls to subroutines works correctly.
   ldi r16, LOW(RAMEND)
   out SPL, r16          ; On avr, SPL and SPH are treated like ports hence
   ldi r16, HIGH(RAMEND) ; mov won't work
   out SPH, r16
   rcall set_portb_high
   ret

set_portb_high:
   ldi r16, 0b11111111
   out DDRB, r16 ; set port OUTPUT
   out PORTB, r16 ; set PORTB high, all pins, PB0...7
   ret
