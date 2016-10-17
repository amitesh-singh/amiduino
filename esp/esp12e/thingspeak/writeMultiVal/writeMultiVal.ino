#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

static const char *ssid = "AMIDUINO";
static const char *passwd = "f53*gmPS";

WiFiClient client;

// this is for writing to thingSpeak channel.
// https://thingspeak.com/channels/113018
unsigned long myChannelNo = 113018;
//Check API key tab
const char *myWriteAPIKey = "writeAPI key";

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
   uint16_t sensorVal = analogRead(A0);

   Serial.print("Sensor Reading: ");
   Serial.print(sensorVal);
   Serial.println(" ADC");

   //Write to thingspeak. There are 8 fields in a channel which allows you to store 8 different pieces of information in a channel.
   // We are writing to channel 1 & 2 here.
   ThingSpeak.setField(1, sensorVal);
   ThingSpeal.setField(2, sensorVal + 2);


   //This will update all Fields at once.
   ThingSpeak.writeFields(myChannelNo,
                          myWriteAPIKey);

   delay(30000);
}
