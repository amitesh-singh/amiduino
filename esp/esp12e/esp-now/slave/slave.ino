//slave

extern "C"
{
#include <espnow.h>
}

#include <ESP8266WiFi.h>

#define CHANNEL 0


void onRecv(uint8_t *macaddr, uint8_t *data, uint8_t len)
{
   char macStr[18];
   snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

   Serial.print("Last Packet Recv from: "); Serial.println(macStr);
   Serial.print("Last Packet Recv Data: "); Serial.println(*data);
   Serial.println("");
}

void setup()
{
   Serial.begin(9600);
   delay(1000);
   for (int i = 0; i < 100; ++i)
     {
        delay(100);
        Serial.print(".");
     }

   Serial.println("");
   Serial.println("Basic ESP now slave example");

   WiFi.mode(WIFI_AP_STA);

   Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());

   // esp now init ..
   if (esp_now_init() == 0)
     {
        Serial.println("Esp now init is done");
     }
   else
     {
        Serial.println("esp now is failed.");
        ESP.restart();
     }

   esp_now_register_recv_cb(onRecv);
}

unsigned long timeout = millis();

void loop()
{
   if (millis() - timeout > 3000)
     {
        Serial.println("Waiting for data ...");
        timeout = millis();
     }
}
