This is the README file for AVRUSBBoot

AVRUSBBoot is a bootloader for the Atmel AVR controllers. It uses a 
firmware-only USB driver to transfer binary data from the PC to the
flash of the controller. Once the AVR is flashed with the bootloader,
no other ISP programmer is needed; then the microcontroller can be
reprogrammed over USB.


LICENSE

The project is built with AVR USB driver by Objective Development, which is
published under a proprietary Open Source license. To conform with this
license, USBasp is distributed under the same license conditions. See the
file "firmware/usbdrv/License.txt" for details. 


BUILDING AND INSTALLING FROM SOURCE CODE

Firmware:
To compile the firmware
1. install the GNU toolchain for AVR microcontrollers (avr-gcc, avr-libc),
2. change directory to firmware/
3. run "make"
4. flash "main.hex" to the ATMega8. E.g. with uisp or avrdude (check
the Makefile option "make avrdude").
You have to change the fuse bits for external crystal, e.g. high byte = 0xc9
and low byte = 0x9f.

Software:
To compile the software:
1. install libusb: http://libusb.sourceforge.net/
2. change directory to software/
3. run "make"

Notes on Windows (Cygwin):
Download libusb-win32-device-bin-x.x.x.x.tar.gz from
http://libusb-win32.sourceforge.net/ and unpack it.
-> copy lib/gcc/libusb.a to lib-path
-> copy include/usb.h to include-path
cd software
make


FILES IN THE DISTRIBUTION

Readme.txt ...................... The file you are currently reading
firmware ........................ Source code of the controller firmware
firmware/usbdrv ................. AVR USB driver by Objective Development
firmware/usbdrv/License.txt ..... Public license for AVR USB driver and USBasp
software ........................ Source code of the host software
circuit ......................... Example circuit diagram
bin/win-driver .................. Windows driver


MORE INFORMATION

For more information on AVRUSBBoot and it's components please visit the
following URLs:

AVRUSBBoot ...................... http://www.fischl.de/avrusbboot/

Firmware-only AVR USB driver .... http://www.obdev.at/products/avrusb/
libusb .......................... http://libusb.sourceforge.net/
libusb-win32 .................... http://libusb-win32.sourceforge.net/


2006-06-25 Thomas Fischl <tfischl@gmx.de>
http://www.fischl.de
