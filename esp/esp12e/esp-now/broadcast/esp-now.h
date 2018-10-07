#ifndef __ESP_NOW___H__
#define __ESP_NOW___H__

#include <Arduino.h>
#include <ESP8266WiFi.h>

extern "C"
{
    #include "espnow.h"
}

class espnow
{
    public:
    int init(WiFiMode_t mode = WIFI_STA, esp_now_role role = ESP_NOW_ROLE_CONTROLLER)
    {
        WiFi.mode(mode);
        delay(1000);

        int ret = esp_now_init();
        if (ret != 0)
        {
            Serial.println("esp now init is failed. restarting..");
            //ESP.restart();
            return ret;
        }

        esp_now_set_self_role(role);

        return ret;
    }

    void addSendCb(esp_now_send_cb_t cb)
    {
        esp_now_register_send_cb(cb);
    }

    void addRecvCb(esp_now_recv_cb_t cb)
    {
        esp_now_register_recv_cb(cb);
    }

    int addPeer(uint8_t *macaddr, esp_now_role role, uint8_t channel, uint8_t *key, uint8_t len)
    {
        return esp_now_add_peer(macaddr, role, channel, key, len);
    }

    int send(uint8_t *macaddr, uint8_t *data, uint8_t len)
    {
        return esp_now_send(macaddr, data, len);
    }
};

#endif