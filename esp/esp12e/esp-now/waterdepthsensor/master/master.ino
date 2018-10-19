#include <Ticker.h>

#include "espnowhelper.h"
#define DEBUG


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     4
#define TFT_RST    0  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     5

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);


static void testdrawtext(char *text, uint16_t color)
{
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

#define WIFI_CHANNEL 1
//30s
#define INTERVAL 30*1000
static const uint8_t buttonPin = 0; //D4?

static const uint8_t slaves_count = 1;

static espnow espmaster;

static uint8_t remoteMac[slaves_count][6] = { 
{ 0x18, 0xFE, 0x34, 0xE2, 0x16, 0x64},
/*
{ 0x18, 0xFE, 0x34, 0xE2, 0x16, 0x64},
{ 0x18, 0xFE, 0x34, 0xE2, 0x16, 0x64},
*/

};

struct __attribute__((__packed__)) waterinfo
 {
     uint8_t sensorid;
     long distance;
     uint8_t percentage;
 };

struct __attribute__((__packed__)) ack
{
    uint8_t sensorid;
};

static volatile ack acks[slaves_count];
static volatile bool reply[slaves_count];
static volatile waterinfo wi[slaves_count];
static volatile unsigned long timestamps[slaves_count];

static void displayBanner()
{
    //TODO: setRotation(!) is the correct one.
    // change it 
    tft.setRotation(3);
    tft.setTextSize(2);

    tft.println("\n");
    tft.setTextColor(ST7735_RED);
    tft.println(" Water Depth");
    tft.println("   Meter");
    Serial.println("\n");
    tft.setTextSize(1);
    Serial.println("");
    tft.setTextColor(ST7735_YELLOW);
    tft.println("         Version 0.1");
    tft.println("\n");
    tft.setTextColor(ST7735_WHITE);
    tft.println("  (C) Amitesh Singh 2018");

    delay(10000);
}

static volatile bool displayOn = true;
Ticker timeout;

static void lcdInit()
{
    // We have ST7735 black tab TFT
    tft.initR(INITR_BLACKTAB); 
    tft.fillScreen(ST7735_BLACK);
}

static void lcdOn()
{
    displayOn = true;
}

static void lcdOff()
{
    tft.fillScreen(ST7735_BLACK);
    displayOn = false;
}

#define TFT_SCREEN_TIMEOUT 60

static void _timeout_cb()
{
    lcdOff();
    timeout.detach();
    #ifdef DEBUG
    Serial.println("Timeout happened, TFT is off.");
    #endif
}

void setup()
{
    timeout.attach(TFT_SCREEN_TIMEOUT, _timeout_cb);

#ifdef DEBUG
    Serial.begin(9600);
    Serial.println("................................");
    Serial.println("Master Display device controller");
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
#endif
    // Display on/off btn
    pinMode(buttonPin, INPUT_PULLUP);

    lcdInit();

    displayBanner();
    tft.fillScreen(ST7735_BLACK);

    testdrawtext("ESP-NOW initializing..\n", ST7735_WHITE);

    if (espmaster.init(WIFI_STA, ESP_NOW_ROLE_COMBO))
    {
        tft.setTextColor(ST7735_RED);
        tft.println("Failed to init ESP-NOW. Restarting...");
    #ifdef DEBUG
        Serial.println("Failed to init ESP-NOW. Restarting ESP8266");
    #endif

        ESP.restart();
    }
#ifdef DEBUG
    Serial.print("Mac Addr: "); Serial.println(WiFi.macAddress());
#endif

    tft.println("Mac Addr:");
    tft.setTextColor(ST7735_GREEN);
    tft.println(WiFi.macAddress());
    delay(5000);
    tft.setTextColor(ST7735_WHITE);
    tft.println("Adding Peers");
    tft.setTextColor(ST7735_GREEN);
    //Add all clients as peer
    for (uint8_t i = 0; i < slaves_count; ++i)
    {
        espmaster.addPeer(remoteMac[i], ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, nullptr, 0);
        reply[i] = false;
        acks[i].sensorid = i + 1;
        timestamps[i] = millis();
        for (u8 j = 0; j < 6; ++j)
            tft.printf("%X:", remoteMac[i][j]);
        tft.println("");
    }

    espmaster.addRecvCb([](uint8_t *macaddr, uint8_t *data, uint8_t len)
    {
#ifdef DEBUG
         Serial.println("Recv_Cb");
         digitalWrite(BUILTIN_LED, LOW);
#endif
        //get the data
        waterinfo *w = (waterinfo *)data;
        if (w->sensorid == 1)
        {
            reply[0] = true;
            memcpy((void *)&wi[0], w, sizeof(waterinfo));
            timestamps[0] = millis();
        }
        else if (w->sensorid == 2)
        {
            reply[1] = true;
            memcpy((void *)&wi[1], w, sizeof(waterinfo));
            timestamps[1] = millis();
        }
        else if (w->sensorid == 3)
        {
            reply[2] = true;
            memcpy((void *)&wi[2], w, sizeof(waterinfo));
            timestamps[2] = millis();
        }
        else
        {
        #ifdef DEBUG
            Serial.println("Invalid packet...");
        #endif
        }
    }
    );
    tft.setTextColor(ST7735_WHITE);
    tft.println("ESP-NOW initialized.");
    tft.println("");
    tft.print("STARTING");
    for (u8 i = 0; i < 10; ++i)
        tft.print("."), delay(1000);
    
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 0);
}

static bool displayStatus = false;
const static u8 tankHeight = 123; //in cm

static uint16_t tankColor = ST7735_RED;


void loop()
{
    for (u8 i = 0; i < slaves_count; ++i)
    {   if (reply[i])
        {
            espmaster.send(remoteMac[i], (u8 *)&acks[i], sizeof(ack));
            reply[i] = false;

            delay(10);
            if (wi[i].distance > tankHeight)
                wi[i].distance = tankHeight;

            wi[i].distance = tankHeight - wi[i].distance;
            if (wi[i].percentage > 100)
                wi[i].percentage = 100;
            wi[i].percentage = 100 - wi[i].percentage;
#ifdef DEBUG
         //TODO: update the readings to serial or i2c/spi screen when you get the update        
            Serial.print("sensor #"); Serial.print(wi[i].sensorid); Serial.print(": ");
            Serial.println(wi[i].distance);
            Serial.print("Percentage: ");
            Serial.println(wi[i].percentage);
            digitalWrite(BUILTIN_LED, HIGH);
#endif
            if (displayOn)
            {
                tft.fillScreen(ST7735_BLACK);
                
                tft.drawRoundRect(5, 5, 50, 105, 5, ST7735_WHITE);
                if (wi[i].percentage >= 0 && wi[i].percentage <= 25)
                    tankColor = ST7735_RED;
                else if (wi[i].percentage > 25 && wi[i].percentage <= 50)
                    tankColor = ST7735_YELLOW;
                else
                    tankColor = ST7735_GREEN;

                if (wi[i].percentage > 0)
                    tft.fillRoundRect(5, 110 - wi[i].percentage, 50, wi[i].percentage, 1, tankColor);
                //show the reading
                tft.setTextSize(2);
                tft.setTextColor(tankColor);
                tft.setCursor(90, 60);
                tft.printf("%d%%", wi[i].percentage);
                tft.setTextSize(1);
                tft.setCursor(60, 100);
                tft.setTextColor(ST7735_WHITE);

                tft.printf("Tank Level:%dcm", wi[i].distance);
                tft.setCursor(0, 0);
            }
        }

        if (millis() - timestamps[i] >= INTERVAL)
        {
            if (displayOn)
            {
                tft.setTextSize(2);
                tft.setCursor(0, 0);
                tft.println("");
                tft.setTextColor(ST7735_RED);
                tft.println("     Sensor");
                tft.println("     Offline");
                tft.setTextColor(ST7735_WHITE);
                tft.setTextSize(1);
            }
#ifdef DEBUG
            Serial.print("----->>>> sensor #"); Serial.print(wi[i].sensorid); Serial.println(" is offline.");
#endif
        }
    }

    displayStatus = digitalRead(buttonPin);

    if (!displayStatus)
    {
        //software debounce
        delay(100);
        if (!digitalRead(buttonPin))
        {
        //Switch On the display and show the reading.
            #ifdef DEBUG

            Serial.println("Button is pressed.");
            Serial.println("Timer started and TFT is on now.");
            #endif

            testdrawtext("Connecting..", ST7735_WHITE);
            tft.println("");
            tft.print("Wait for a moment.");
            
            lcdOn();
            timeout.detach();
            timeout.attach(TFT_SCREEN_TIMEOUT, _timeout_cb);
        }

    }    
//    delay(1000);
}
