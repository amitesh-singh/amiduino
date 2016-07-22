.include "../defs/m16def.inc"

_start:
   jmp main

main:
   ;first enable the internal pull up resistors
   clr r16
   out DDRB, r16 ; all PortB0...7 are for input only

   ldi r16, 0xff
   out PORTB, r16 ; input pullup high
loop:
   ; read the input at whole PB0..7 ports
   in r16, PINB
   ; if one of port is pressed then its not equal to 0xff
   cpi r16, 0xff
   breq loop
