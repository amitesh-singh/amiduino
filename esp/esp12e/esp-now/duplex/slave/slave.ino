//slave 

#include <ESP8266WiFi.h>

extern "C" 
{
  #include <espnow.h>
}


// this is the MAC Address of the remote ESP 
  uint8_t remoteMac[] = {0x18, 0xFE, 0x34, 0xE1, 0xAC, 0x6A}; // pin


#define WIFI_CHANNEL 4

volatile boolean recv;

void recv_cb(u8 *macaddr, u8 *data, u8 len)
{
  recv = true;
  Serial.print("recv_cb ");
  Serial.println(len); 
};

void send_cb(uint8_t* mac, uint8_t sendStatus) 
{
  Serial.println("send_cb ");
};

void setup() 
{
  Serial.begin(9600); Serial.println();

  WiFi.mode(WIFI_STA); // Station mode for esp-now 
  WiFi.disconnect(); // This is needed to avoid conflict with Router.

  Serial.printf("This mac: %s, ", WiFi.macAddress().c_str()); 
  Serial.printf("target mac: %02x%02x%02x%02x%02x%02x", remoteMac[0], remoteMac[1], remoteMac[2], 
  remoteMac[3], remoteMac[4], remoteMac[5]); 
  Serial.printf(", channel: %i\n", WIFI_CHANNEL); 

  if (esp_now_init() != 0) Serial.println("*** ESP_Now init failed");

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  esp_now_add_peer(remoteMac, ESP_NOW_ROLE_CONTROLLER, WIFI_CHANNEL, NULL, 0);

  esp_now_register_recv_cb(recv_cb);
  esp_now_register_send_cb(send_cb);
}

char hello[] = "hello world";

void loop() 
{
  if (recv)
  {
    Serial.println();    
    esp_now_send(NULL, (u8*)hello, sizeof(hello)); // NULL means send to all peers
    recv = false;
  }
  delay(900);
}
