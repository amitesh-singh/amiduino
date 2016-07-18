.include "../defs/m16def.inc"

.def temp = r16
.def leds = r17
.def switch = r18

.cseg
.org 0

_start:
   rjmp main

ledson:
   com switch ; all bits are flipped
   out PORTD, switch
   rjmp comebackagain + 2; ; or at release?

main:
    ; portb - all input - switch connected
    ; portd - all output - leds connected
    clr temp
    out DDRB, temp

    com temp
    out DDRD, temp

    out PORTB, temp ; enable internal pullups

    loop:
    ; check for button to be pressed
      press:
         in switch, PINB
         cpi switch, 0xff ; switch == 0xff;
         breq press

      ;do something when either of the switch is pressed
      comebackagain:
         rjmp ledson

      release:
         in switch, PINB
         cpi switch, 0xff ; switch == 0xff
         brne release

      rjmp loop
