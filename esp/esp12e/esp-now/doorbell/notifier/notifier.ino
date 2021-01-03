
#include "espnowhelper.h"

//disable it in production to save time and power
#define DEBUG 

constexpr uint8_t clients_count = 1;
constexpr uint8_t retry_attemps = 5;
constexpr uint8_t WIFI_CHANNEL = 1;

static espnow esp_now;
static uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct __attribute__((__packed__)) data_t
{
    uint32_t batteryVoltage;
} data ;


uint8_t sent = 0;
uint8_t reply = 0;

static void print_mac(uint8_t *macaddr)
 {  
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
    
    Serial.println(macStr);
 }

//ADC code to read battery voltage
ADC_MODE(ADC_VCC);
void setup()
{
    #ifdef DEBUG
    Serial.begin(115200);
    Serial.println();
    Serial.println();
    #endif

    esp_now.init();

    data.batteryVoltage = ESP.getVcc();
    delay(100);
    //Add peer
    esp_now.addPeer(broadcastAddr, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL);
    esp_now.addSendCb([](uint8_t *macaddr, uint8_t status)
    {
        sent++;
        #ifdef DEBUG
        Serial.println("packet sent");
        #endif
    }
    );

    esp_now.addRecvCb([](uint8_t *macaddr, uint8_t *incomingdata, uint8_t len)
    {
        reply++;
        #ifdef DEBUG
        Serial.print("Got ACK from: ");
        print_mac(macaddr);
        #endif
    }
    );
    esp_err_t result = esp_now.send(broadcastAddr, (uint8_t *)&data, sizeof(data));
    delay(1000);
    //ESP.deepSleep(0);
}

uint8_t retry_count = 0;

void loop()
{    
    if (retry_count > retry_attemps || (sent >= clients_count && reply >= clients_count))
    {
        #ifdef DEBUG 
            Serial.println("Going to sleep for forever.");
            delay(100);
        #endif 
        
        ESP.deepSleep(0);
    }
    else
    {
        retry_count++;
        #ifdef DEBUG
        Serial.print("retry count: "); Serial.println(retry_count);
        #endif
        esp_now.send(broadcastAddr, (uint8_t *)&data, sizeof(data));
        delay(1000);
    }
}
