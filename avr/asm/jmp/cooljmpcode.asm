.include "../defs/m16def.inc"

.cseg
.org 0

_start:
	clr r17
	rjmp main
	
main:
	
	clr r0
	ldi ZL, low(jmpTable)  ; r30
	ldi ZH, high(jmpTable) ;r31
	add ZL, r17
	adc ZH, r0  ; add value of r17  to Z
	ijmp ; jump to jumpTable but we change the jmp little bit on run time.
	nop
	nop
	rjmp main

jmpTable:
	rjmp isZero
	rjmp isOne
	rjmp isTwo



isZero:
isOne:
isTwo:
	nop
	nop
	inc r17
	cpi r17, 3
	breq resetr17
rjmp main

resetr17:
	clr r17
	rjmp main

