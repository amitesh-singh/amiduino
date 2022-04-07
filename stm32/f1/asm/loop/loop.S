@ https://www.mikrocontroller.net/articles/ARM-ASM-Tutorial
# arm-none-eabi-as -g loop.s -o loop.o
# arm-none-eabi-ld loop.o -o loop.elf -Ttext=0x8000000
.syntax unified
.cpu cortex-m3
.thumb

.word 0x20000400
.word 0x080000ed
.space 0xe4

nop @ do nothing 
b . @  endless loop
