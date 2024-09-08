# Cyclone 4 (bought from china) EP4CE6E22C8
## IC
- EP4CE6E22C8 pins are 3.3v tolerant. But not 5v.  
## Installation


1. install quartus prime lite which has cyclone4 support on arch linux
2. the bin will be installed at ~/intelFPGA_lite/23.1std/quartus/bin
3. add ~/intelFPGA_lite/23.1std/quartus/bin into your $PATH
4. issue `quartus` to start the IDE

## New Project

### using `quartus` IDE

- File -> New Project Wizard 
- choose EP4CE6E22C8

### important files
- <project name>.qpf is the project file and use this file to open an existing project using `quartus ide`
- <project name>.qsf contains the details like DEVICE EP4CE6E22C8 and pins assigments


### using Makefile
- Makefile can be used to compile and flash 
```
# Use PROJECT as a variable, default to not_switch_led if not provided
PROJECT ?= led

all: compile fit asm

compile:
	quartus_map --read_settings_files=on --write_settings_files=off $(PROJECT) -c $(PROJECT)

fit:
	quartus_fit --read_settings_files=on --write_settings_files=off $(PROJECT) -c $(PROJECT)

asm:
	quartus_asm --read_settings_files=on --write_settings_files=off $(PROJECT) -c $(PROJECT)

program:
	quartus_pgm -c 1 -m jtag -o "p;output_files/$(PROJECT).sof"
```

usage:

```
make PROJECT=led
```
