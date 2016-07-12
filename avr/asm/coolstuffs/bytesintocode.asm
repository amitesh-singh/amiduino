;we add the binary code directly into code.

.dw 0xe001 ; move r16, 0x01
.dw 0x0f00 ; add r16, r16
.dw 0xcfff ; rjmp 0x0002
