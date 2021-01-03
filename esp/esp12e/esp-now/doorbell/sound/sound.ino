#include "espnowhelper.h"

//TODO: disable it later
#define DEBUG
constexpr uint8_t WIFI_CHANNEL = 1;

struct __attribute__((__packed__)) data_t
 {
     uint32_t batteryVoltage; 
 } data ;

static espnow esp_now;

struct doorbell
{
    bool volatile pressed;
};

doorbell doorbell;

void system_init()
{
    memset(&doorbell, 0, sizeof(doorbell));
}

void setup()
{
    Serial.begin(115200);
    
    system_init();

    esp_now.init(WIFI_STA, ESP_NOW_ROLE_SLAVE);    
    esp_now.addRecvCb([](uint8_t *macaddr, uint8_t *incomingdata, uint8_t len)
    {
        memcpy(&data, incomingdata, sizeof(data));
        doorbell.pressed = true;
    }
    );
}

void loop()
{
    if (doorbell.pressed)
    {
        Serial.println("Doorbell is pressed");
        //play some sound?
        doorbell.pressed = false;
    }   
}