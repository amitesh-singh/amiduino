#include <ESP8266WiFi.h>

extern "C" 
{
  #include <espnow.h>
}

// this is the MAC Address of the remote ESP 
 static uint8_t remoteMac[] = {  0x18, 0xFE, 0x34, 0xE2, 0x16, 0x64};

#define WIFI_CHANNEL 4

volatile boolean recv;

void recv_cb(u8 *macaddr, u8 *data, u8 len)
{
  recv = true;
  Serial.print("recv_cb ");
  Serial.println(len); 
  if (!esp_now_is_peer_exist(macaddr))
  {
    esp_now_add_peer(macaddr, ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, NULL, 0);
    Serial.println("Added new Peer.");
  }
};

void send_cb(uint8_t* mac, uint8_t sendStatus) 
{
  Serial.println("send_cb ");
};

void setup() 
{
  Serial.begin(9600);
  Serial.println();

  WiFi.mode(WIFI_STA); // Station mode for esp-now 
  WiFi.disconnect();

  Serial.printf("This mac: %s, ", WiFi.macAddress().c_str()); 
  Serial.printf("target mac: %02x%02x%02x%02x%02x%02x", remoteMac[0], remoteMac[1], remoteMac[2], 
  remoteMac[3], remoteMac[4], remoteMac[5]); 
  Serial.printf(", channel: %i\n", WIFI_CHANNEL); 

  if (esp_now_init() != 0) Serial.println("*** ESP_Now init failed");

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  //esp_now_add_peer(remoteMac, ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, NULL, 0);

  esp_now_register_recv_cb(recv_cb);
  esp_now_register_send_cb(send_cb);
}

char hello[] = "hello world";

void loop() 
{
  esp_now_send(NULL, (u8*)hello, sizeof(hello)); // NULL means send to all peers
  delay(1000);

}