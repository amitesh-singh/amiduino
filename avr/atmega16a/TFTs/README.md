### Tips

I have two ili3941 and one ST7735 (RED PCB) (1.8")  
Adafruit libraries for these two displays work fine.  
ST7735 power supply is 5V tolerant but the logic pins are not 5v tolerant.
Apply logic level converter for this.  

#### ST7735
Connections  
AVR --> Atmega16  

AVR     TFT  
5v  ---- Vcc  
GND ---- GND

CLK ---- SCK  (PB7)  
SDA ---- MOSI  (PB5)  
RS/DC ---- PB3  
RST  ----- PB2 or RST pin of atmega16  
CS  -----  PB4  

