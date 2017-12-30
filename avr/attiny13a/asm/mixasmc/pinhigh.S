#include <avr/io.h>
//the following needed to subtract 0x20 from i/o address
//refer: http://msoe.us/taylor/tutorial/ce2810/candasm
#define _SFR_ASM_COMPAT 1  /* Not sure when/if this is needed */
#define __SFR_OFFSET 0

.org 0x0

.section .text
.global loop
.global set_pb0_high

loop:
  rjmp loop

set_pb0_high:
  ldi r16, 1
  out DDRB, r16
  out PORTB, r16
  ret

