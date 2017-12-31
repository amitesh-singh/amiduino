#define __SFR_OFFSET 0
   .nolist
#include <avr/io.h>
   .list

//refer: http://msoe.us/taylor/tutorial/ce2810/candasm
#define _SFR_ASM_COMPAT 1  /* Not sure when/if this is needed */
//the following needed to subtract 0x20 from i/o address
// code will not work if you don't set this.

.org 0x0

.section .text

.global set_pb0_high
.global set_pb0_low
.global led_init
.global delay_250ms

led_init:
   ldi r16, 1
   out DDRB, r16
   ret

set_pb0_high:
  ldi r16, 1
  out PORTB, r16
  ret

set_pb0_low:
   ldi r16, 0
   out PORTB, r16
   ret

//4.8Mhz = 4.8e6 cycles
// for 250ms, we need to have some code which gives 1.2e6 cycles
// 2.4e6/255/255 = 36.90888119953864
// so we can have 3 loops like below
// calculation
//  loop3: gets called  128*(1 + 1 + 1 + 128*(1 + 1 + 1) + 128*(36*(1 + 1 + 1))) -> we are ignoring brne 2 cycles on jmp for simplicity
//  128*(1 + 1 + 1) + 128*128*(1 + 1 + 1) + 128*128*(36*(1 + 1))/4.8e6 = 0.25608s -> approximate.
// lets do more accurate
//
// 128*(1 + 1) + 127*1 + 1*2 + 128*128*(1 + 1) + 128*127*1 + 128*1*2 + 128*128*36*1 + 128*128*35*1 + 128*128*1*2 = 1245697
// 1245697/4.8e6 = 0.25952020833333334 s
//
delay_250ms:
   ldi r17, 128 //1clk

   loop1:
      ldi r18, 128 //1ck

      loop2:
         ldi r19, 36 //1clk

         loop3:
            dec r19 //1
            brne loop3 //2clk if jmp else 1clk

         dec r18 //1
         brne loop2 // break if r18 != 0 // 2clk if jmp else 1clk

      dec r17 //1
      brne loop1 // 2clk if jmp else 1clk
   ret
