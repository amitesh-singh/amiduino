/*
  * esp-now helper class
  * Copyright (C) 2018  Amitesh Singh <singh.amitesh@gmail.com>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef __ESP_NOW__HELPER__H__
#define __ESP_NOW__HELPER__H__

#include <ESP8266WiFi.h>
#include <espnow.h>

typedef int esp_err_t;

class espnow
{
    public:
    esp_err_t init(WiFiMode_t mode = WIFI_STA, esp_now_role role = ESP_NOW_ROLE_CONTROLLER)
    {
        WiFi.mode(mode);
        WiFi.disconnect();

        esp_err_t ret = esp_now_init();
        if (ret != 0)
        {
            //Serial.println("esp now init is failed. restarting..");
            //ESP.restart();
            return ret;
        }
        //role can be ESP_NOW_ROLE_IDLE = 0, ESP_NOW_ROLE_CONTROLLER, ESP_NOW_ROLE_SLAVE, ESP_NOW_ROLE_COMBO,  ESP_NOW_ROLE_MAX
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

    esp_err_t addPeer(uint8_t *macaddr, esp_now_role role, uint8_t channel, uint8_t *key = nullptr, uint8_t len = 0)
    {
        return esp_now_add_peer(macaddr, role, channel, key, len);
    }


    esp_err_t send(uint8_t *macaddr, uint8_t *data, uint8_t len)
    {
        return esp_now_send(macaddr, data, len);
    }
};

#endif
