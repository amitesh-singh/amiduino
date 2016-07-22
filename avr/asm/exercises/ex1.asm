;Design a loop using a conditional branch instruction thatwill repeat 255 times. What value will be used to initialize102
;the loop counter? What initial value would be used to repeat
;the loop 256 times? Include two nop instuctions in the body
;of the loop. If the microcontroller is running at 1 MHz, how
;long will the code take to execute 256 iterations (answer in
;milliseconds)?

.include "../defs/m16def.inc"

.cseg
.org 0

_start:
	rjmp main

main:

clr r16
loop_256times:
   nop     ; 1
	nop     ; 1
	inc r16   ; 1
	cpi r16, 0   ; 1
	brne loop_256times  ; if loop then 2, else 1

rjmp main

; maths
;   [1 +  1 + 1 + 1 + 2] * 255 + [1 +  1 + 1 + 1 + 1] * 1 = 
;   6 * 255 + 5 = 1535 cycles = by this loop
; time taken by the loop for 1 Mhz speed MCU = 1535 * 10 to power 6 = 1.5 ms
;
;
