.DEF A = r16
.DEF B = r17
.DEF C = r18 ; r16 - r31 most used since half the work load of using r0 - r15
; constants
.SET MYAGE = 22
.EQU MYBDAY = 12

.include "../defs/m16def.inc"

.ORG 0000 ; tell assember to put our codes into 0000 location.

ldi A, 100
ldi B, 200
ldi C, MYAGE

