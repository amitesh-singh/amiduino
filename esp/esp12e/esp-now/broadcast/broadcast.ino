#include "esp-now.h"
//This does not work on esp8266 yet.


espnow esp;

typedef struct esp_now_peer_info_t
{
    uint8_t peer_addr[6];
    uint8_t channel;
    esp_now_role role;
} esp_now_peer_info_t __attribute__((packed));

#define CHANNEL 0

esp_now_peer_info_t slave;

bool manageSlave()
{
    esp_now_peer_info_t *peer = &slave;

    if (slave.channel == CHANNEL)
    {
        bool exists = esp_now_is_peer_exist(peer->peer_addr);
        if (exists)
        {
            Serial.println("Already paired.");
            return true;
        }
        else
        {
            //not paired.
            Serial.printf("not paired: %x:%x:%x:%x:%x:%x\r\n", peer->peer_addr[0],
            peer->peer_addr[1],
            peer->peer_addr[2],
            peer->peer_addr[3],
            peer->peer_addr[4],
            peer->peer_addr[5]);

            int addStatus = esp.addPeer(peer->peer_addr, peer->role, peer->channel, NULL, 0);
            if (addStatus == 0)
            {
                Serial.println("pairing is success.");
                return true;    
            }
            else
            {
                Serial.println("Pairing is unsuccessful");
                return false;
            }
        }
    }
}

void setup()
{
    Serial.begin(9600);
    delay(100);

    for (int i = 0; i < 100; ++i)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("");

    Serial.println("Broadcast example espnow");

    if (esp.init(WIFI_STA, ESP_NOW_ROLE_COMBO))
    {
        Serial.println("Failed to init esp-now");
        ESP.restart();
    }

    memset(&slave, 0, sizeof(slave));
    for (int i = 0; i < 6; ++i)
    {
        slave.peer_addr[i] = 0xff;
    }
    slave.channel = CHANNEL;
    slave.role = ESP_NOW_ROLE_COMBO;

    esp.addSendCb([](uint8_t *macaddr, uint8_t status)
    {
        char macStr[18];
         snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
		    macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
	    Serial.print("Last Packet sent to: "); Serial.println(macStr);
        Serial.print("Last Packet Send Status: "); Serial.println(status == 0 ? "Delivery Success" : "Delivery Fail");
    }

    );

    esp.addRecvCb([](u8 *macaddr, u8 *data, u8 len)
    {
        char macStr[18];
	    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
		    macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
	    Serial.print("Last Packet Recv from: "); Serial.println(macStr);
	    Serial.print("Last Packet Recv Data: "); Serial.println(*data);
        Serial.println("");
    }
    );
    manageSlave();
}

void send(uint8_t data)
{
    int sendStatus = esp.send(slave.peer_addr, &data, sizeof(data));
    if (sendStatus == 0)
    {
        Serial.println("Data sent:");
    }
    else
    {
        Serial.println("Failed to send Data.");
    }
}

unsigned int counter = 0;

void loop()
{
    send(counter++);

    delay(3000);
}
