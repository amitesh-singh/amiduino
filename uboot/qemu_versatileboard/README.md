Refer to this: 
https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
https://balau82.wordpress.com/2010/03/10/u-boot-for-arm-on-qemu/

My notes
--------

- QEMU supports  VersatilePB  board. We can check this by issueing following command
```
qemu-system-arm -machine help

```
 VersatilePB contains ARM926EJ-S core, which has four UART ports.
 UART0 address is mapped to 0x101f1000 

 We can write following code for printing Hello world using uart0

```
 volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;
 
void print_uart0(const char *s) {
 while(*s != '\0') { /* Loop until end of string */
 *UART0DR = (unsigned int)(*s); /* Transmit char */
 s++; /* Next char */
 }
}
 
void c_entry() {
 print_uart0("Hello world!\n");
}
```

The QEMU emulator is written especially to emulate Linux guest systems; for this reason its startup procedure is implemented specifically: the -kernel option loads a binary file (usually a Linux kernel) inside the system memory starting at address 0x00010000. The emulator starts the execution at address 0x00000000, where few instructions (already in place) are used to jump at the beginning of the kernel image. The interrupt table of ARM cores, usually placed at address 0x00000000, is not present, and the peripheral interrupts are disabled at startup, as needed to boot a Linux kernel. Knowing this, to implement a working emulation I need to considerate a few things:

The software must be compiled and linked to be placed at 0x00010000
I need to create a binary image of our program
I can ignore interrupt handling for now

startup.S
```
.global _Reset
_Reset:
 LDR sp, =stack_top
 BL c_entry
 B .
```
test.ld

```
ENTRY(_Reset)
SECTIONS
{
 . = 0x10000;
 .startup . : { startup.o(.text) }
 .text : { *(.text) }
 .data : { *(.data) }
 .bss : { *(.bss COMMON) }
 . = ALIGN(8);
 . = . + 0x1000; /* 4kB of stack memory */
 stack_top = .;
}
```

```
$ arm-none-eabi-as -mcpu=arm926ej-s -g startup.s -o startup.o
$ arm-none-eabi-gcc -c -mcpu=arm926ej-s -g test.c -o test.o
$ arm-none-eabi-ld -T test.ld test.o startup.o -o test.elf
$ arm-none-eabi-objcopy -O binary test.elf test.bin
```

To run the program,

```
$ qemu-system-arm -M versatilepb -m 128M -nographic -kernel test.bin
```

to exit QEMU, press “Ctrl + a” and then “x”.

It is possible also to debug the program using the CodeSourcery version of gdb, because QEMU implements a gdb connector using a TCP connection. To do so, I run the emulator with the correct options as follows:

```
$ qemu-system-arm -M versatilepb -m 128M -nographic -s -S -kernel test.bin
```


uboot for ARM on QEMU
---------------------
clone uboot: https://github.com/u-boot/u-boot

```
make qemu_arm_config ARCH=arm CROSS_COMPILE=arm-none-eabi-

```

This command configures U-Boot to be compiled for the VersatilePB board. Then compile and build with:


```
make all ARCH=arm CROSS_COMPILE=arm-none-eabi-

```

The compilation will create a u-boot.bin binary image. To simulate, run:

```
qemu-system-arm -M virt -m 128M -nographic -bios ./u-boot.bin

```


mkimage -A arm -C none -O linux -T kernel -d test.bin -a 0x00100000 -e 0x00100000 test.uimg

cat uboot.bin test.bin > flash.bin


qemu-system-arm -M virt -m 128M -nographic -kernel flash.bin

This does not boot up due to removal of support for versatilepb
