#include <avr/io.h>

//the following needed to subtract 0x20 from i/o address
//refer: http://msoe.us/taylor/tutorial/ce2810/candasm
//#define _SFR_ASM_COMPAT 1  /* Not sure when/if this is needed */
#define __SFR_OFFSET 0

.org 0x0

.section .text

.global loop
.global set_pb0_high
.global set_pb0_low
.global blink_led
.global led_init

led_init:
   ldi r16, 1
   out DDRB, r16
   ret

loop:
  rcall blink_led
  rjmp loop

set_pb0_high:
  ldi r16, 1
  out PORTB, r16
  ret

set_pb0_low:
   ldi r16, 0
   out PORTB, r16
   ret

blink_led:
   rcall set_pb0_low
   ldi r16, 250
   push r16
   rcall _delay_1s
   rcall set_pb0_high
   ret


