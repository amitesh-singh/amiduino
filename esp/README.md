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

#### Power settings

 In my experiments, putting 220 uF capacitor on power rail actually stablizes the noisy power.
 Also put a 104 (100 nF/0.1 uF) accross Vcc and GND stablizes the esp.

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
 100k ohm and 47k ohm would be fine. (200 ohm and 100 ohm were not good enough because of larger current)

### GPIO pins

1. The maximum source current of GPIO is about 12mA.

### how to use gpio pins

http://www.forward.com.au/pfod/ESP8266/GPIOpins/ESP8266_01_pin_magic.html

### how to code without arduino ide which is actually pretty nice.

`cd ~  
 git clone https://github.com/plerup/makeEspArduino.git  
 cd makeEspArduino  

 add this line in .bashrc  
 alias espmake="make -f ~/makeEspArduino/makeEspArduino.mk"

 Now go to any esp code folder and issue espmake  
 To upload the code, use espmake upload.
 
 `


### ESP SDK tutorial
1. http://smallbits.marshall-tribe.net/blog/
2. http://www.instructables.com/id/How-to-Write-ESP8266-Firmware-From-Scratch-using-E/?ALLSTEPS


#### ESP wifi reconnect issue

//https://github.com/esp8266/Arduino/issues/2186  
// sometimes wifi does not reconnect.. the below  
// 3 lines fixes it.  
// rebooting router is the only option .. pretty annoying.  
//WiFi.persistent(false); --> This was crashing esp, don't know why  
// The below lines seems to work fine.  
WiFi.mode(WIFI_OFF);  
WiFi.mode(WIFI_STA);  
WiFi.setOutputPower(0);  
WiFi.begin(ssid, passwd);  

