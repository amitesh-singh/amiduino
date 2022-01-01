#include <vector>

#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t kok[16]= {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
uint8_t key[16]= {0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44};

//controller mac address is 18:FE:34:E1:AC:6A
uint8_t controller_mac[6] = {0x18, 0xFE, 0x34, 0xE1, 0xAC, 0x6A};
uint8_t slave_mac[6] = {0xA2, 0x55, 0x55, 0x55, 0x55, 0x55};

struct peer_info_t
{
    u8 mac[6];
    u8 channel = 1;
};
std::vector<peer_info_t> peers;

static void print_mac(uint8_t *macaddr)
 {  
    char macStr[18];
    Serial.print("Packet received from: ");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4],         macaddr[5]);
    Serial.println(macStr);
 }


void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    delay(1000);
    Serial.println("");
    Serial.println("Starting...");
    WiFi.mode(WIFI_STA);
    Serial.print("Mac Addr:");
    Serial.println(WiFi.macAddress());

    //initialize esp-now
    if (esp_now_init() != 0)
    {
        Serial.println("Failed to initialize esp-now");
    }

    esp_now_set_kok(kok, 16);

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    esp_now_register_send_cb([](u8 *mac, u8 status) {
        //print_mac(mac);
    });

    esp_now_register_recv_cb([](u8 *mac, u8 *data, u8 len){
        if (!esp_now_is_peer_exist(mac))
        {
            peer_info_t peer;
            memcpy(peer.mac, mac, 6);
            Serial.println("new peer is added");
            peers.push_back(peer);
            esp_now_add_peer(mac, ESP_NOW_ROLE_COMBO, peer.channel, key, 16);
            esp_now_set_peer_key(mac, key, 16);
        }
        else print_mac(mac);
        Serial.print("peers size = ");
        Serial.printf("%d\n", peers.size());
    });
}

void loop()
{
    digitalWrite(LED_BUILTIN, 1);
    delay(1000);
    digitalWrite(LED_BUILTIN, 0);
    delay(1000);
    Serial.println("blink-");
    Serial.println(WiFi.macAddress());
}