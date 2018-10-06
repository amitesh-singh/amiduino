// Esp now example

#include <ESP8266WiFi.h>

extern "C"
{
    #include <espnow.h>
    #include <user_interface.h>
}

static bool retry = false;

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("Serial setup done.");
    
    WiFi.mode(WIFI_STA);
    Serial.println("ESP-now basic master example.");
    //print mac address of ESP master in station aka STA mode.
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());

    //initialize ESP-now with fallback logic
    if (!esp_now_init())
    {
        Serial.println("Esp now is initialized.");
    }
    else
    {
        Serial.println("Esp now init is failed. restarting..");
        ESP.restart();
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    //esp now init done.

    //Add a callback on send 
    esp_now_register_send_cb([](uint8_t *macaddr, uint8_t status)
    {
        static uint32_t ok = 0, fail = 0;
        if (status == 0)
        {
            ok++;
            retry = false;
            Serial.println("ESPNOW: ACK_OK");
        }
        else
        {
            fail++;
            Serial.println("ESPNOW: ACK_FAIL");
        }

        Serial.printf("Success: %lu/%lu \r\n", ok, ok+fail);
    }
    );

    // mac of slave connected at CHANNEL 0
    static uint8_t remoteMac[] = {  0xA0, 0x20, 0xA6, 0x00, 0xF1, 0xCC};

#define CHANNEL 0

    int addStatus = esp_now_add_peer(remoteMac, ESP_NOW_ROLE_CONTROLLER, CHANNEL, NULL, 0);
    if (addStatus == 0)
    {
        Serial.println("Pairing is success.");
    }
    else
    {
        Serial.println("Pairing is failed.");
    }
}


void loop()
{

}