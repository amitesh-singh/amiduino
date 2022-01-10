// use https://packetsender.com/download#show to send msg
// available for linux as well.

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

static const char ssid[] = "Aviaarav-2.4G";
static const char passwd[] = "poojasingh";

WiFiUDP udp;
const char *replyBuffer = "Hello there!";
IPAddress remote(192, 168, 1, 11);
constexpr const uint16_t remotePort = 15001;

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, passwd);

    while ((WiFi.status()) != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("WiFi connected to "); Serial.println(WiFi.SSID());
    Serial.print("IP Address: "); Serial.println(WiFi.localIP());

    //udp.begin(localUdpPort);
    //Serial.print("Listening at udp port: "); Serial.println(localUdpPort);
}

void loop()
{
   //send a reply
   //for broadcast use beginMulticastPacket(...)
   udp.beginPacket(remote, remotePort);
   udp.write(replyBuffer);
   udp.endPacket();

   delay(1000);
}
