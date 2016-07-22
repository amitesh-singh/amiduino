; This examples shows how to handle bouncing when button is pressed
;
;

.include "../defs/m16def.inc"

.def temp = r16
.def leds = r17
.def switch = r18

.cseg
.org 0

_start:
   ; stack init
   ldi temp, LOW(RAMEND)
   out SPL, temp
   ldi temp, HIGH(RAMEND)
   out SPH, temp

   rjmp main

_delay_1ms:
   ldi r21, 80 ; 1
   upper_loop:
      ldi r20, 250 ; 1
      int_loop:
         dec r20 ; 1
         cpi r20, 0 ; 1
         brne int_loop ; jmp 2 and passthrough 1

      dec r21 ; 1
      cpi r21, 0 ; 1
      brne upper_loop ; jmp 2 and passthrough 1
   ret

ledson:
   com switch ; all bits are flipped
   eor leds, switch
   out PORTB, leds
   rjmp comebackagain + 2; ; or at release?

main:
    ; portb - all input - switch connected
    ; portd - all output - leds connected
    clr temp
    out DDRD, temp

    com temp
    out DDRB, temp

    out PORTD, temp

    loop:
    ; check for button to be pressed
      press:
         in switch, PIND
         cpi switch, 0xff ; switch == 0xff;
         breq press

      rcall _delay_1ms
      ; do check for button pressed or not
      press_2:
         in switch, PIND
         cpi switch, 0xff
         breq press_2

      ;do something when either of the switch is pressed
      comebackagain:
         rjmp ledson

      release:
         in switch, PIND
         cpi switch, 0xff ; switch == 0xff
         brne release

      rjmp loop
