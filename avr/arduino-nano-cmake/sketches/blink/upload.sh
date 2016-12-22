sudo chmod 666 /dev/ttyUSB0

#/home/ami/Downloads/arduino-1.6.12/hardware/tools/avr/bin/avrdude -C/home/ami/Downloads/arduino-1.6.12/hardware/tools/avr/etc/avrdude.conf -v -patmega328p -carduino -P/dev/ttyUSB0 -b115200 -D -Uflash:w:proj.hex:i

/home/ami/Downloads/arduino-1.6.12/hardware/tools/avr/bin/avrdude -C/home/ami/Downloads/arduino-1.6.12/hardware/tools/avr/etc/avrdude.conf -v -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:blink.hex:i 
