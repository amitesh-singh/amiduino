#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

const char * ssid = "Aviaarav-2.4G";
const char * password = "poojasingh";

AsyncUDP udp;

void setup()
{
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);

   while(WiFi.status() != WL_CONNECTED)
     {
        delay(1000);
        Serial.print(".");
     }
   Serial.printf("Connected to %s\r\n", WiFi.localIP());

   bool ret = udp.connect(IPAddress(192, 168, 1, 11), 16003);
   if  (!ret)
     {
        Serial.println("Failed to connect");
     }
   else
     {
        udp.onPacket([](AsyncUDPPacket packet) {
                     Serial.print("UDP Packet Type: ");
                     Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
                     Serial.print(", From: ");
                     Serial.print(packet.remoteIP());
                     Serial.print(":");
                     Serial.print(packet.remotePort());
                     Serial.print(", To: ");
                     Serial.print(packet.localIP());
                     Serial.print(":");
                     Serial.print(packet.localPort());
                     Serial.print(", Length: ");
                     Serial.print(packet.length());
                     Serial.print(", Data: ");
                     Serial.write(packet.data(), packet.length());
                     Serial.println();
                     //reply to the client
                     packet.printf("Got %u bytes of data", packet.length());
                     });
     }

   udp.print("Hello Server");
}

void loop()
{
   udp.print("Hello Server");
   delay(1000);
}
