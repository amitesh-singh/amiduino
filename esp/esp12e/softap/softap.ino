#include <ESP8266WiFi.h>

void setup()
{
   WiFi.mode(WIFI_STA);

   Serial.begin(115200);

   bool res = WiFi.softAP("amiteshsingh", "password");

   if (res)
     Serial.println("Connected..");
   else
     Serial.println("failed to connect");
}


void loop()
{
   Serial.printf("Stations connected = %d\r\n", WiFi.softAPgetStationNum());
   delay(3000);
}
