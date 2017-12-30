#include <avr/io.h>

//the following needed to subtract 0x20 from i/o address
#define __SFR_OFFSET 0
.section .data
.org 0x0

.section .text
.global main

main:
   ldi r16, 1
   out DDRB, r16

   out PORTB, r16

   loop:
      rjmp loop
.end
