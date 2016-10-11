## Notes

### ESP 01 

#### GPIO pins numbering

pin      arduino pin number in code.  
GPIO_0 - 0  
GPIO_1 - 1, inbuilt LED (which is TxD)  
GPIO_2 - 2  

#### Arduino IDE settings
 Board: "Generic ESP8266 Module"  
 Flash Mode: "DIO"  
 Flash Frequency: 40 Mhz  
 CPU Frequency: 80 Mhz  
 Flash Size: `512 K(64 SPIFFS)`  
 Debug Port: Disabled  
 Debug Level: None  
 Reset method: "ck"  
 Upload Speed: 115200  

### ESP 12E

#### GPIO pins numbering

 TODOs  

#### Arduino IDE settings
 Board: "Generic ESP8266 Module"  
 Flash Mode: "DIO"  
 Flash Frequency: 40 Mhz  
 CPU Frequency: 80 Mhz  
 Flash Size: `4M(1 SPIFFS) or 4M (3 SPIFFS)`  
 Debug Port: Disabled  
 Debug Level: None  
 Reset method: "ck"  
 Upload Speed: 115200  

### ADC reading on ESP12E

 It has a ADC port (use `A0` in programming, `analogRead(A0)`).
 It can only read between 0 to 1V so you would need to use voltage divider for this.
 200 ohm and 100 ohm would be fine.

### GPIO pins

1. The maximum source current of GPIO is about 12mA.

### how to use gpio pins

http://www.forward.com.au/pfod/ESP8266/GPIOpins/ESP8266_01_pin_magic.html

