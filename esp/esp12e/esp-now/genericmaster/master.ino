// Esp now example
// mac for master is 18:FE:34:E1:AC:6A
//slave mac address is 18:FE:34:E2:16:64
// This does not work on esp8266, but works on esp32.

#include <ESP8266WiFi.h>

extern "C"
{
#include <espnow.h>
#include <user_interface.h>
}

#define CHANNEL 0

static bool retry = true;
uint8_t data = 1;

// mac of slave connected at CHANNEL 0
//static uint8_t remoteMac[]= {  0x18, 0xFE, 0x34, 0xE2, 0x16, 0x64};
static uint8_t slaveMac[6];

void setup()
{
   Serial.begin(9600);
   delay(2000);
   Serial.println("Serial setup done.");

   // esp works in 3 modes, STATION and Soft Access mode (AP) and Soft AP + STAT 
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

   esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
   //esp now init done.

   //This does not work on esp8266, although it seems to work on esp32
   esp_now_register_recv_cb([](u8 *macaddr, u8 *data, u8 len)
   {
     if (!esp_now_is_peer_exist(macaddr))
     {
       Serial.println("Adding Peer.");
       memcpy(slaveMac, macaddr, 6);
       esp_now_add_peer(macaddr, ESP_NOW_ROLE_COMBO, CHANNEL, NULL, 0);
     }
   }
   );
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
   // channel is from 0 to 14
   // and at max, 20 nodes can be connected.
   /*
   //below code is not needed since we are adding peer on recv callback.
   int addStatus = esp_now_add_peer(remoteMac, ESP_NOW_ROLE_CONTROLLER, CHANNEL, NULL, 0);

   if (addStatus == 0)
     {
        Serial.println("Pairing is success.");
     }
   else
     {
        Serial.println("Pairing is failed.");
     }
     */
}

void sendData()
{
   data++;
   int result = esp_now_send(slaveMac, &data, sizeof(data));
   Serial.print("Send Command: ");
   if (result ==0)
     {
        Serial.println("Success " + String(result));
     }
   else
     {
        Serial.println("Failed " + String(result));
     }
   delay(100);
}

void loop()
{
   while (retry)
     {
        sendData();
        delay(100);
     }

   retry = true;
   delay(10000);
}
