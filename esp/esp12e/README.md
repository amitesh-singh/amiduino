### Notes

#### Connection for flashing via arduino ide

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

