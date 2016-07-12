; blink 1 second without using timer

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
  
  ldi r16, 0b00000001
  out DDRB, r16
  clr r17

loop:
  out PORTB, r16
  rcall _delay_1s
  out PORTB, r17
  rcall _delay_1s
  rjmp loop

ret

; below function gives 8024195 cycles ~~ 1.003024 seconds ; not very accurate though.
_delay_1s:
   ldi r18, 64 ; 1 clk cycle
   ldi r19, 125; ; 1 clk cycle
   ldi r20, 250; 1 clk
  ; 8 Mhz = 8 x 10^6 cycles
  ; can't use r16 or r17 as it is getting used so lets use r18, r19 and r10 if required
  delay_1:
  delay_2:
  delay_3:
	nop         ; 1 clk
	dec r20 ; 1 clk
	brne delay_3; 2 clk if jmp  and 1 clk if continue
   
   dec r19 ;1 clk
   brne delay_2 ; 2 clk

   dec r18 ; 1 clk
   brne delay_1 ; 2clk

ret

  
	

