# Use PROJECT as a variable, default to not_switch_led if not provided
PROJECT ?= uart_tx_timer

all: compile fit asm

compile:
	quartus_map --read_settings_files=on --write_settings_files=off $(PROJECT) -c $(PROJECT)

fit:
	quartus_fit --read_settings_files=on --write_settings_files=off $(PROJECT) -c $(PROJECT)

asm:
	quartus_asm --read_settings_files=on --write_settings_files=off $(PROJECT) -c $(PROJECT)

program:
	quartus_pgm -c 1 -m jtag -o "p;output_files/$(PROJECT).sof"
