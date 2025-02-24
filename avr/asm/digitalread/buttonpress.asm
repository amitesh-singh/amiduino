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
   out PORTB, switch
   rjmp comebackagain + 2; ; or at release?

main:
    ; portb - all input - switch connected
    ; portd - all output - leds connected
    clr temp
    out DDRD, temp

    com temp
    out DDRB, temp

    out PORTD, temp ; enable internal pullups

    loop:
    ; check for button to be pressed
      press:
         in switch, PIND
         cpi switch, 0xff ; switch == 0xff;
         breq press

      ;do something when either of the switch is pressed
      comebackagain:
         rjmp ledson

      release:
         in switch, PIND
         cpi switch, 0xff ; switch == 0xff
         brne release

      rjmp loop
