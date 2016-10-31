// (C) Amitesh Singh

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#define __ESP8266_EX__

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735_ESP.h> // Hardware-specific library
#include <SPI.h>

#include "icons.h"
// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     4
#define TFT_RST    0  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     5

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

#define TFT_SCLK 13
#define TFT_MOSI 14
static const char ssid[] = "AMIDUINO";
static const char passwd[] = "wifi password";

static void drawImage(const char *weather)
{
   const int8_t posX = 74, posY = 80;
   if (!strcmp(weather, "Clouds"))
   {
      tft.drawXBitmap(posX, posY, Cloudy_bits, Cloudy_width,
                      Cloudy_height, ST7735_WHITE);
   }
   else if (!strcmp(weather, "Clear"))
   {
      tft.drawXBitmap(posX, posY, Clear_bits, Clear_width,
                      Clear_height, ST7735_RED);
   }
   else if (!strcmp(weather, "Rainy"))
   {

      tft.drawXBitmap(posX, posY, Light_Rain_bits, Light_Rain_width,
                      Light_Rain_height, ST7735_WHITE);

   }
   else if (!strcmp(weather, "WiFi"))
   {
      tft.drawXBitmap(posX - 10, posY, WiFi_Logo_bits, WiFi_Logo_width,
                      WiFi_Logo_height, ST7735_CYAN);
   }
}

void setup()
{
   Serial.begin(115200);
   Serial.println();
   Serial.println();
   Serial.println();
   tft.initR(INITR_BLACKTAB);
   delay(100);
   tft.fillScreen(ST7735_BLACK);
   delay(100);
   tft.println("");
   tft.println("");
   tft.println("");
   tft.println("Booting...");
   drawImage("WiFi");

   delay(10000);

   tft.fillScreen(ST7735_BLACK);

   tft.setCursor(0, 0);
   tft.setTextWrap(true);
   //tft.setTextColor(ST7735_BLACK);
   tft.setTextSize(2);
   tft.println("Weather");
   tft.println("Station");

   tft.setTextSize(1);
   tft.setTextColor(ST7735_RED);
   tft.setCursor(10, 50);
   tft.println("- (C) Amitesh Singh");
   tft.setTextColor(ST7735_BLUE);
   tft.setCursor(0, 100);
   Serial.println("");
   Serial.println("");
   tft.println("Connecting to WiFi");

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

   while (WiFi.status() != WL_CONNECTED)
   {
      delay(500);
      Serial.print(".");
   }
   delay(10000);
   tft.fillScreen(ST7735_BLACK);
   delay(100);
   tft.setCursor(0, 0);
   tft.setTextColor(ST7735_RED);
   tft.setTextWrap(true);
   Serial.println("");
   Serial.print("Connected to ");
   tft.setTextSize(1);
   Serial.println(WiFi.localIP());

   //tft.setRotation(1); //set landscape

   tft.setCursor(0, 0);
   //tft.setTextColor(ST7735_RED);
   tft.setTextWrap(false);
   tft.setTextSize(1);
   tft.println("Connected to ");
   tft.setTextSize(1);
   tft.println("");
   tft.println("");
   tft.println(WiFi.localIP().toString());
   delay(10000);
   tft.fillScreen(ST7735_BLACK);
}

static int httpCode;
String payload;
static const char *cities[] = { "New%20Delhi", "Noida",
                                  "Ghaziabad", "Varanasi"
                              };
static uint8_t cityIndex = 0;

void loop()
{
   if (WiFi.status() != WL_CONNECTED)
   {
      Serial.println("Wifi is not connected. Resetting ESP...");
      // Reset the ESP
      ESP.restart();
      return;
   }
   if (cityIndex >= 4) cityIndex = 0;

   HTTPClient http;
   String prefix = "http://api.openweathermap.org/data/2.5/weather?q=";
   prefix += cities[cityIndex++];
   prefix += ",in&appid=1a702a15a2f46e405e61804cf67c0d30&units=metric";

   //http.begin("http://api.openweathermap.org/data/2.5/weather?q=New%20Delhi,in&appid=1a702a15a2f46e405e61804cf67c0d30&units=metric");
   http.begin(prefix);
   delay(1000);

   httpCode = http.GET();
   delay(1000);
   if (httpCode > 0)
   {
      if (httpCode == HTTP_CODE_OK)
        {
           tft.fillScreen(ST7735_BLACK);
           delay(100);
           payload = http.getString();
           Serial.println(payload);
           Serial.print("Size = ");
           Serial.println(payload.length());
        }

   }
   else
      Serial.println("[HTTP]: Failed to get data");
   http.end();

   if (httpCode == HTTP_CODE_OK)
   {
      //Note: It did not work with StaticJsonBuffer.
      //StaticJsonBuffer<572> jsonBuffer
     DynamicJsonBuffer jsonBuffer;
     JsonObject& root = jsonBuffer.parseObject(payload);
     if (!root.success())
     {
         Serial.println("parseObject() failed");
         return;
     }
     uint8_t humidity = root["main"]["humidity"];
     float temp = root["main"]["temp"];
     float windSpeed = root["wind"]["speed"];
     const char *weather = root["weather"][0]["main"];
     Serial.print(temp);
     Serial.print(humidity);
     Serial.println("");
     tft.setCursor(0, 10);
     tft.setTextColor(ST7735_BLUE);
     tft.setTextWrap(false);
     tft.setTextSize(1);
     tft.println("Temperature:");
     delay(500);
     //   tft.setCursor(20, 20);
     tft.setTextColor(ST7735_RED);
     tft.setTextSize(2);
     tft.print(temp);
     tft.println(" C");
     tft.println("");
     delay(500);
     tft.setTextSize(1);
     //tft.setCursor(20, 30);
     tft.setTextColor(ST7735_BLUE);
     tft.println("Humidity:");
     delay(500);
     tft.setTextSize(2);
     //tft.setCursor(20, 40);
     tft.setTextColor(ST7735_WHITE);
     tft.println(humidity);
     tft.println("");
     delay(500);
     tft.setTextSize(1);
     tft.setTextColor(ST7735_BLUE);
     tft.println("wind:");
     delay(100);
     tft.setTextSize(2);
     tft.setTextColor(ST7735_GREEN);
     tft.println(windSpeed);
     tft.println("");
     tft.setTextSize(2);
     const char *name = root["name"];
     tft.setTextColor(ST7735_YELLOW);
     tft.println(name);
     Serial.println("Weather:");
     Serial.println(weather);

     drawImage(weather);
   }

   //wait for 60 seconds
   delay(60000);
}
