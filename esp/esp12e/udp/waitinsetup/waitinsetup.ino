// use https://packetsender.com/download#show to send msg
// available for linux as well.

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>

static const char ssid[] = "Aviaarav-2.4G";
static const char passwd[] = "poojasingh";

WiFiUDP udp;
static const int localUdpPort = 15001;

unsigned char recvBuffer[255];
const char *replyBuffer = "Hello there!";
int packetSize;

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

    if (!MDNS.begin("udp_broadcaster"))
     {
        Serial.println("Failed to set mdns");
     }
   else
     {
        Serial.println("set up mdns as udp_broadcaster");
     }

    udp.begin(localUdpPort);
    Serial.print("Listening at udp port: "); Serial.println(localUdpPort);

    //in this way, we can configure all configs by communicating to a server
    while (1)
      {
         packetSize = udp.parsePacket();
         if (packetSize)
           {
              Serial.printf("Recv packet from %s, port: %d\r\n",
                            udp.remoteIP().toString().c_str(), udp.remotePort());

              int len = udp.read(recvBuffer, 255);
              if (len)
                {
                   recvBuffer[len] = 0;
                }
              Serial.printf("packet content: %s\r\n", recvBuffer);
           }
      }
}


void loop()
{

   /*
   //send a reply
   udp.beginPacket(udp.remoteIP(), udp.remotePort());
   udp.write(replyBuffer);
   udp.endPacket();
    */
}
