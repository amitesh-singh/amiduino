# necessary settings for the chip we use
set(MCU atmega328p)
set(F_CPU 16000000UL)
set(BAUD 115200UL)

# set the programmer and the device to use (default is to use simavr)
#set(PROGRAMMER usbasp)
set(PROGRAMMER arduino)
#set(PROGRAMMER dragon_dw)
set(SERIAL_DEV /dev/cu.SLAB_USBtoUART)
#set(SERIAL_DEV /tmp/simavr-uart0)
#set(SERIAL_DEV usb)
#set(SERIAL_DEV /dev/cu.usbserial-A96TDJ7N)

# only needed if we do floating point math and want to print floats with printf
#set(EXTRA_LIBS "-lm -lprintf_flt")
# show a list of libs linked
set(EXTRA_LIBS "-Wl,--trace")

# if you want to compile Arduino sketches, set this to the Arduino SDK dir (1.6.5)
set(ARDUINO_SDK_PATH "/home/ami/arduino-1.6.12" CACHE STRING "Arduino SDK Path")
