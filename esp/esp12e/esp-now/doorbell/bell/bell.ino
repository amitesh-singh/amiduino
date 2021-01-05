#include "espnowhelper.h"
#include <Ticker.h>
#include "tone.h"
#include "config.h"

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
Ticker blinker;

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
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);
}

static void print_mac(uint8_t *macaddr)
{
   char macStr[18];
   Serial.print("Packet received from: ");
   snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
   Serial.println(macStr);
}

static void blink_led(uint16_t timeout = 250)
{
    blinker.detach();
    digitalWrite(BUILTIN_LED, LOW);
    blinker.once_ms(timeout, []()
    {
        digitalWrite(BUILTIN_LED, HIGH);
    }
    );
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

sound::tone mytone;
int play_sound_count = 0;

void loop()
{
    if (doorbell.pressed)
    {
        play_sound_count = BELL_RING_COUNT;
        //blink led whenever doorbell is pressed
        blink_led();

        Serial.println("Doorbell is pressed");
        //play some sound?
        doorbell.pressed = false;
        Serial.print("Sensor battery voltage (V):");
        Serial.println(doorbell.batteryVoltage/1000.0);
        Serial.println();
        reply_data_t reply_data;
        
        reply_data.id = SensorId::SOUND;

        //send reply to SENSOR 
        esp_now.send(doorbell.macaddr, (uint8_t *)&reply_data, sizeof(reply_data));
        delay(250);
    }

    if (play_sound_count > 0)
    {
       --play_sound_count;
       mytone.play_imperial_march(D2);
       delay(100);
    }
}
