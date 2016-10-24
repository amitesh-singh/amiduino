; blink 1 second without using timer
; Author: ami <singh.amitesh at gmail.com>
; device atmega16a, 8 Mhz

.device ATmega16

.set DDRB = 0x17
.set PORTB = 0x18
.set SPL = 0x3d
.set SPH = 0x3e ; check m16def.inc
.set RAMEND = $45f

.cseg
.org 0

; initialize STACK
_start:
  ldi r16, LOW(RAMEND)
  out spl, r16
  ldi r16, HIGH(RAMEND)
  out sph,  r16

  sbi DDRB, 0    ; can use ldi r16, 0b00000001 ; out DDRB, r16 ; since we are
                 ; only setting 1 byte, just use sbi instead.
                 ; to clear the bit use cbi; e.g. cbi DDRB, 0
  clr r17

loop:
  sbi PORTB, 0
  rcall _delay_1s
  ; out PORTB, r17 , better to use cbi PORTB, 0 instead
  cbi PORTB, 0

  rcall _delay_1s
  rjmp loop
;ret ; remove this also since we are not gonna reach here. its waste of 2 bytes for us.

; below function gives 8024195 cycles ~~ 1.003024 seconds ; not very accurate though.
_delay_1s:
   ldi r18, 64 ; 1 clk cycle
  ; 8 Mhz = 8 x 10^6 cycles
  delay_1:
   ldi r19, 125; ; 1 clk cycle
  delay_2:
  ldi r20, 250; 1 clk
  delay_3:
  nop         ; 1 clk
	dec r20 ; 1 clk
	brne delay_3; 2 clk if jmp  and 1 clk if continue

   dec r19 ;1 clk
   brne delay_2 ; 2 clk

   dec r18 ; 1 clk
   brne delay_1 ; 2clk

ret
