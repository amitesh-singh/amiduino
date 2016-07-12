;we add the binary code directly into code.

.dw 0xe001 ; move r16, 0x01
.dw 0x0f00 ; add r16, r16
.dw 0xcfff ; rjmp 0x0002


.db 0x03, 0x95 ; below two statements are similar to what this statement does
; .dw 0x9503     
; inc r 16
