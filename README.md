# amiduino
Arduino Samples/Learning

adruniho simulator on windows
-------------------------------

http://sheepdogguides.com/arduino/aht9simsimm.htm

awesome simulators
------------------
https://123d.circuits.io

Links:
------

1. how to program ATTiny85 microcontroller using Arduino http://highlowtech.org/?p=1695

2. how to make gamepad: http://www.varesano.net/blog/fabio/serial-communication-arduino-and-processing-simple-examples-and-arduino-based-gamepad-int
3. shift registers
  http://www.codeproject.com/Articles/144606/Arduino-Platform-Working-with-Shift-Registers
4. some advanced stuffs: avr dude etcs
https://pragprog.com/magazines/2011-04/advanced-arduino-hacking
5. Arduino bases games on 16x2 LCDs
  a. http://www.raywenderlich.com/66560/arduino-tutorial-creating-pong

  http://www.circuitbasics.com/how-to-set-up-an-lcd-display-on-an-arduino/

6. communication:
http://www.circuitbasics.com/basics-uart-communication/
http://www.circuitbasics.com/basics-of-the-spi-communication-protocol
http://www.circuitbasics.com/basics-of-the-i2c-communication-protocol/

7. how to add your library "manage library"
  https://github.com/arduino/Arduino/wiki/Library-Manager-FAQ
  https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification#library-metadata
  https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification
  

how to setup on linux/ubuntu
-----------------------------
1. download the linux binary from arduino.cc website and install.
2. while uploading the sketch, you might get following errors.
  ===
  permission denied: /dev/ttyUSB0 or /dev/ttyUSBX where X is a number from
0 to N.
To solve this problem,
  #sudo chmod 666 /dev/ttyUSB0 or /dev/ttyUSBX
 you might get error in opening serial console on arduino, change
the owner of the file to user ami.
 #  sudo chown ami /dev/ttyUSB0


install colormake if u want to show warnings/errors in color

sudo apt-get install colormake

$colormake # instead of make


