; note: this command does not do much on avr. its only for quick demo/brush ups

.include "../defs/m16def.inc"

.dseg
c_lab: .byte 1 ; declare c_lab of size 1 byte in SRAM
a_lab: .byte 2 ; declate a_lab of size 2 bytes in SRAM

.cseg
.org 0
_start:
   ; set up our stack ptr so that calls to subroutines works correctly.
   ldi r16, LOW(RAMEND)
   out SPL, r16          ; On avr, SPL and SPH are treated like ports hence
   ldi r16, HIGH(RAMEND) ; mov won't work
   out SPH, r16

   rjmp main

main:
   ldi r16, 'a'
   sts c_lab, r16  ; c_lab = 'a' in C

   ldi r16, 1
   ldi r17, 0
   sts a_lab, r16
   sts a_lab + 1, r17  ; a_lab: 0 - high, 1 - low

   ; lds - load direct from data memory
   lds r24, a_lab
   lds r25, a_lab + 1
   addiw r25:r24, 1

   sts a_lab, r24
   sts a_lab + 1, r25

   loop:
      rjmp loop
