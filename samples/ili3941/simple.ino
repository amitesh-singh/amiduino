
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 6
#define TFT_CS 5
#define  TFT_MOSI 11
#define TFT_CLK 13
#define TFT_RST 4
#define TFT_MISO 12

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

unsigned long setText()
{
   unsigned long start = micros();
   tft.fillScreen(ILI9341_BLACK);
   tft.setCursor(0, 0);
   tft.setTextColor(ILI9341_WHITE);
   tft.setTextSize(4);
   tft.println("Happy");
   tft.setTextColor(ILI9341_YELLOW);
   tft.println("Birthday");
   tft.setTextColor(ILI9341_RED);
   tft.println("To");
   tft.setTextColor(ILI9341_GREEN);
   tft.setTextSize(6);
   tft.println("Aarav");
   return micros() - start;
}

void setup()
{
   Serial.begin(9600);
   Serial.println("ILI9341 Test!"); 

   tft.begin();

   // read diagnostics (optional but can help debug problems)
   uint8_t x = tft.readcommand8(ILI9341_RDMODE);
   Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
   x = tft.readcommand8(ILI9341_RDMADCTL);
   Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
   x = tft.readcommand8(ILI9341_RDPIXFMT);
   Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
   x = tft.readcommand8(ILI9341_RDIMGFMT);
   Serial.print("Image Format: 0x"); Serial.println(x, HEX);
   x = tft.readcommand8(ILI9341_RDSELFDIAG);
   Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);

   Serial.println(F("Benchmark                Time (microseconds)"));
   delay(10);

   Serial.println(setText());
}

void loop()
{
   for(uint8_t rotation=0; rotation<4; rotation++)
     {
        tft.setRotation(rotation);
        setText();
        delay(1000);
     }
}
