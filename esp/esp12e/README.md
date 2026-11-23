# Notes

## esp8266 nodemcu board

### How to know the flash size 
```
$ esptool --port /dev/ttyUSB0 flash-id
esptool v5.6.0
Connected to ESP8266 on /dev/ttyUSB0:
Chip type:          ESP8266EX
Features:           Wi-Fi, 160MHz
Crystal frequency:  26MHz
MAC:                18:fe:34:e2:0c:71

Stub flasher running.

Flash Memory Information:
=========================
Manufacturer: e0
Device: 4016
Detected flash size: 4MB

Hard resetting via RTS pin...

```

## Connection for flashing via arduino ide

This is for the DIY board i made for esp12e chip.
The inbuilt led is at port 2

## Pin Mapping

GPIO5  - 5  
GPIO4 - 4   
GPIO0 - 0
GPIO2 - 2
GPIO15 - 15
GPIO12 - 12
GPIO13 - 13
GPIO14 - 14
GPIO16 - 16
ADC - i don't know



1. connect FTD-232
2.
   CH_PD - 3.3v  
   GPIO_0 - GND (directly)
   GPIO_15 - GND
   common ground with FTD and chip
   
   start the device and then quickly put flash

   remember D0 - GPIO16
3. For booting..
   CH_PD - high
   GPIO_0 - high
   GPIO_15 - LOW
4. After booting, GPIO_0 and GPIO_15 can be used as normal GPIO.
Use weak pull up/down for this purpose.
a 10k ohms resistor would be enough.


