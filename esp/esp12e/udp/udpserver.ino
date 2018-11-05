// use https://packetsender.com/download#show to send msg
// available for linux as well.

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

static const char ssid[] = "AndroidHotspot4095";
static const char passwd[] = "";

WiFiUDP udp;
static const int localUdpPort = 2805;

unsigned char recvBuffer[255];
const char *replyBuffer = "Hello there!";
int packetSize;

void setup()
{
    Serial.begin(9600);
    
    WiFi.begin(ssid, passwd);

    while ((WiFi.status()) != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("WiFi connected to "); Serial.println(WiFi.SSID());
    Serial.print("IP Address: "); Serial.println(WiFi.localIP());

    udp.begin(localUdpPort);
    Serial.print("Listening at udp port: "); Serial.println(localUdpPort);
}

void loop()
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

        //send a reply
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.write(replyBuffer);
        udp.endPacket();
    }
}