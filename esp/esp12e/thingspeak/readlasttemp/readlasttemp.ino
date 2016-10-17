#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

static const char *ssid = "AMIDUINO";
static const char *passwd = "yourwifi password";

WiFiClient client;

// this is for reading following thingSpeak channel.
// https://thingspeak.com/channels/12397
unsigned long weatherStationChannelNo = 12397;
unsigned int temperatureFieldNo = 4;

void setup()
{
   Serial.begin(115200);

   delay(8000);

   WiFi.begin(ssid, passwd);
   delay(5000);

   ThingSpeak.begin(client);
   delay(100);
}

void loop()
{
   //This reads following channel https://thingspeak.com/channels/12397
   // notice Field 4 - which is a temperature in F.
   float temp = ThingSpeak.readFloatField(weatherStationChannelNo, temperatureFieldNo);

   Serial.print("Current temp is: ");
   Serial.print(temp);
   Serial.println(" degrees F");

   delay(30000);
}
