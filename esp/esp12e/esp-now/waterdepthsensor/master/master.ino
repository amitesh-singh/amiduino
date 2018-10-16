#include "espnowhelper.h"
#define DEBUG

//#define SLAVES_COUNT 3
#define WIFI_CHANNEL 1

static const uint8_t slaves_count = 1; 

espnow espmaster;

uint8_t remoteMac[slaves_count][6] = { 
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
 };

struct __attribute__((__packed__)) ack
{
    uint8_t sensorid;
};

static volatile ack acks[slaves_count];
static volatile bool reply[slaves_count];
volatile waterinfo wi[slaves_count];

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Master Display device controller");
#endif

    if (espmaster.init(WIFI_STA, ESP_NOW_ROLE_COMBO))
    {
    #ifdef DEBUG
        Serial.println("Failed to init ESP-NOW");
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
    }
    espmaster.addRecvCb([](uint8_t *macaddr, uint8_t *data, uint8_t len)
    {
#ifdef DEBUG
         Serial.println("Recv_Cb");
#endif
        //get the data
        waterinfo *w = (waterinfo *)data;
        if (w->sensorid == 1)
        {
            reply[0] = true;
            memcpy((void *)&wi[0], w, sizeof(waterinfo));
        }
        else if (w->sensorid == 2)
        {
            reply[1] = true;
            memcpy((void *)&wi[1], w, sizeof(waterinfo));
        }
        else if (w->sensorid == 3)
        {
            reply[2] = true;
            memcpy((void *)&wi[2], w, sizeof(waterinfo));
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

void loop()
{
    for (u8 i = 0; i < slaves_count; ++i)
    {   if (reply[i])
        {
            espmaster.send(remoteMac[i], (u8 *)&acks[i], sizeof(ack));
            reply[i] = false;
            delay(50);
        }

#ifdef DEBUG
        Serial.print("sensor #"); Serial.print(wi[i].sensorid); Serial.print(": ");
        Serial.println(wi[i].distance);
#endif
    }
    delay(1000);
}
