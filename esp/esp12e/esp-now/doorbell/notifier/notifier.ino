
#include "espnowhelper.h"

//disable it in production to save time and power
#define DEBUG 

constexpr uint8_t WIFI_CHANNEL = 1;

static espnow esp_now;

static uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct __attribute__((__packed__)) data_t
{
    uint32_t batteryVoltage;

} data ;

//ADC code to read battery voltage
ADC_MODE(ADC_VCC);

void setup()
{
    #ifdef DEBUG
    
    Serial.begin(115200);
    
    #endif

    esp_now.init();

    data.batteryVoltage = ESP.getVcc();
    //Add peer
    esp_now.addPeer(broadcastAddr, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL);
    esp_now.addSendCb([](uint8_t *macaddr, uint8_t status)
    {
        #ifdef DEBUG
        Serial.println("Delivery Status:");
        if (status == 0)
        {
            Serial.println("Delivery Succees.");
        }
        else
            Serial.println("Delivery Fail.");
        #endif
    }
    );
    esp_err_t result = esp_now.send(broadcastAddr, (uint8_t *)&data, sizeof(data));
    delay(1000);
    
    #ifdef DEBUG 
    
    Serial.println("Going to sleep for forever.");
    delay(100);
    
    #endif 

    ESP.deepSleep(0);
}

void loop()
{    
}
