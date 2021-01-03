#include "espnowhelper.h"

//update this whenever new device is added in the device tree.
enum SensorId
{
    SOUND,
    BALCONY
};

//TODO: disable it later
#define DEBUG
constexpr uint8_t WIFI_CHANNEL = 1;

static espnow esp_now;

struct doorbell
{
    bool volatile pressed;
    uint32_t batteryVoltage;
    uint8_t macaddr[6];
};

struct __attribute__((__packed__)) reply_data_t
{
    uint8_t id;
};

doorbell doorbell;

void system_init()
{
    memset(&doorbell, 0, sizeof(doorbell));
}

static void print_mac(uint8_t *macaddr)
{
   char macStr[18];
   Serial.print("Packet received from: ");
   snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
   Serial.println(macStr);
}

void setup()
{
    Serial.begin(115200);
    
    system_init();

    esp_now.init(WIFI_STA, ESP_NOW_ROLE_SLAVE);    
    esp_now.addRecvCb([](uint8_t *macaddr, uint8_t *incomingdata, uint8_t len)
    {
        print_mac(macaddr);
        memcpy(&doorbell.macaddr, macaddr, sizeof(doorbell.macaddr));
        memcpy(&doorbell.batteryVoltage, incomingdata, sizeof(doorbell.batteryVoltage));
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
        Serial.print("Sensor battery voltage (V):");
        Serial.println(doorbell.batteryVoltage/1000.0);
        reply_data_t reply_data;
        
        reply_data.id = SensorId::SOUND;

        //send reply to SENSOR 
        esp_now.send(doorbell.macaddr, (uint8_t *)&reply_data, sizeof(reply_data));
        delay(1000);
    }
}
