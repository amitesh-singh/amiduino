#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>

constexpr const char ssid[] = "Aviaarav-2.4G";
constexpr const char passwd[] = "poojasingh";

WiFiUDP udp;
constexpr uint16_t port = 15001;

IPAddress broadcast(255, 255, 255, 255);

void setup()
{
   Serial.begin(115200);
   Serial.println("");
   Serial.println("");
   Serial.println("");
   //fix reconnect issue.
   //WiFi.mode(WIFI_OFF);
   WiFi.mode(WIFI_STA);
   //WiFi.setOutputPower(0);
   WiFi.begin(ssid, passwd);

   while (WiFi.status() != WL_CONNECTED)
     {
        delay(1000);
        Serial.print(".");
     }
   Serial.println("Connected");
   //print all info related to wifi.
   Serial.print("IP Address: ");
   Serial.println(WiFi.localIP());

   Serial.print("Broadcast Address: ");
   Serial.print(broadcast);
   Serial.print(":");
   Serial.println(port);

   if (!MDNS.begin("udp_broadcaster"))
     {
        Serial.println("Failed to set mdns");
     }
   else
     {
        Serial.println("set up mdns as udp_broadcaster");
     }
}

void sendUDP(const String &msg)
{
   udp.beginPacketMulticast(broadcast, port, WiFi.localIP());
   udp.write(msg.c_str(), msg.length());
   udp.endPacket();
}

void loop()
{
   sendUDP("Hello");
   delay(1000);
}
