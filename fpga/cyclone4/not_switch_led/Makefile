all: compile fit asm

compile:
	quartus_map --read_settings_files=on --write_settings_files=off not_switch_led -c not_switch_led

fit:
	quartus_fit --read_settings_files=on --write_settings_files=off not_switch_led -c not_switch_led

asm:
	quartus_asm --read_settings_files=on --write_settings_files=off not_switch_led -c not_switch_led

program:
	quartus_pgm -c 1 -m jtag -o "p;output_files/not_switch_led.sof"