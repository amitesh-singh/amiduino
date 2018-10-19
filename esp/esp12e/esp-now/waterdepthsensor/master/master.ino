#include "espnowhelper.h"
#define DEBUG

#define WIFI_CHANNEL 1
//30s
#define INTERVAL 30*1000
static const uint8_t buttonPin = 5; //D4?

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

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println("................................");
    Serial.println("Master Display device controller");
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
#endif
    // Display on/off btn
    pinMode(buttonPin, INPUT_PULLUP);

    if (espmaster.init(WIFI_STA, ESP_NOW_ROLE_COMBO))
    {
    #ifdef DEBUG
        Serial.println("Failed to init ESP-NOW. Restarting ESP8266");
    #endif

        ESP.restart();
    }
#ifdef DEBUG
    Serial.print("Mac Addr: "); Serial.println(WiFi.macAddress());
#endif

    //Add all clients as peer
    for (uint8_t i = 0; i < slaves_count; ++i)
    {
        espmaster.addPeer(remoteMac[i], ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, nullptr, 0);
        reply[i] = false;
        acks[i].sensorid = i + 1;
        timestamps[i] = millis();
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
}

static bool displayStatus = false;

void loop()
{
    for (u8 i = 0; i < slaves_count; ++i)
    {   if (reply[i])
        {
            espmaster.send(remoteMac[i], (u8 *)&acks[i], sizeof(ack));
            reply[i] = false;

            delay(10);
#ifdef DEBUG
         //TODO: update the readings to serial or i2c/spi screen when you get the update        
            Serial.print("sensor #"); Serial.print(wi[i].sensorid); Serial.print(": ");
            Serial.println(wi[i].distance);
            Serial.print("Percentage: ");
            Serial.println(wi[i].percentage);
            digitalWrite(BUILTIN_LED, HIGH);
#endif
        }

        if (millis() - timestamps[i] >= INTERVAL)
        {
#ifdef DEBUG
            Serial.print("----->>>> sensor #"); Serial.print(wi[i].sensorid); Serial.println(" is offline.");
#endif
        }
    }
    displayStatus = digitalRead(buttonPin);

    if (!displayStatus)
    {
        //Switch On the display and show the reading.
        #ifdef DEBUG

        Serial.println("Button is pressed.");
        
        #endif
    }    
//    delay(1000);
}
