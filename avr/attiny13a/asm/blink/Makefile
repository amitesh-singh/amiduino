######################################################################
#  Assembler bassert macro for ATtinyX
#  Wed Oct 22 21:39:02 2014	Warren W. Gay VE3WWG
#  LICENSE: GPL
######################################################################

DEVICE	?= attiny13a
PDEV	?= t13
F_CPU	?= 4800000	
NDEBUG	?= 0

ifneq	($(NDEBUG),0)
NDEBUG	= -DNDEBUG
else
NDEBUG	=
endif

OPTZ	   = -Os $(DEFNS)
DEFNS	   = -DF_CPU=$(F_CPU) $(NDEBUG)
COPTS	   = $(OPTZ)

.SUFFIXES: .hex .elf

CC	= avr-gcc -Wall $(OPTZ) -mmcu=$(DEVICE)
AS	= avr-as

.c.o:
	$(CC) -c $(COPTS) $< -o $@

.S.o:
	$(CC) -x assembler-with-cpp -c -g $(DEFNS) $< -o $@ -Wa,-a=$*.list

.c.s:
	$(CC) -S $< -o $@

.elf.hex:
	@rm -f $*.hex
	avr-objcopy -j .text -j .data -O ihex $< $@
	avr-size --format=avr --mcu=$(DEVICE) $<

OBJ	= blink.o main.o

all:	main.hex 

main.elf: $(OBJ)
	$(CC) $(OBJ) -o main.elf

blink.o: Makefile blink.S

clean:
	rm -f *.o

clobber: clean
	rm -f *.elf *.hex sim.out .errs.t *.list 

disasm:	main.elf
	avr-objdump -d main.elf
