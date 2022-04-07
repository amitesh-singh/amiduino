#cat -e -t -v  makefile
loop: loop.o
	arm-none-eabi-ld loop.o -o loop.elf -Ttext=0x8000000

loop.o: loop.S
	arm-none-eabi-as -g loop.S -o loop.o

hex: loop.elf
	arm-none-eabi-objcopy -O ihex loop.elf loop.hex
binary: loop.elf
	arm-none-eabi-objcopy -O binary loop.elf loop.bin
	
clean:
	rm -rf *.o
