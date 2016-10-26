### Tips

Check the back side of board. U2 is HC245 (NXP), it means it is ST7735 display (1.8")  
Also it means that i can use 5v and i don't need level transistors.
DC is same as RS

my 1.8 display's pin configuration are
Vcc  -- 5v  
GND -- GND  

CLK  -- SCLK
SDA -- MOSI
RS/DC  --  
RST --  
CS  --  


There are two ways to use this display.  

1.Slow speed   

`#define TFT_SCLK 4    
#define TFT_MOSI 5    
#define TFT_CS   6    
#define TFT_DC   7    
#define TFT_RST  8    `

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
  
2.  FAST way using FAST HW SPI

#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

#define TFT_SCLK 13
#define TFT_MOSI 11
