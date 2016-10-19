#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define DEBUG

#include "DHT.h"

#define DHTPIN 4

#define DHTTYPE DHT11

static const char ssid[] = "AMIDUINO";
static const char passwd[] = "f53*gmPS";

WiFiClient client;

DHT dht(DHTPIN, DHTTYPE);


static const char *myWriteKey = "BHYXFOGPKH8J73JT";
void setup()
{
#ifdef DEBUG
   Serial.begin(115200);
   while (!Serial)
   {
     //wait for serial to get ready or else you get crash, especially on esp12
   }
#endif


#ifdef DEBUG
   Serial.println("start logging to thingspeak");
#endif

   dht.begin();

   WiFi.begin(ssid, passwd); // connect with router
   while (WiFi.status() != WL_CONNECTED)
   {
      delay(500);
#ifdef DEBUG
      Serial.print(".");
#endif
   }

#ifdef DEBUG
   Serial.println("");
   Serial.print("Connected to ");
   Serial.println(WiFi.localIP());
#endif

   ThingSpeak.begin(client);
   delay(100);
}

static const uint32_t channelNo = 113018;
void loop()
{
   delay(2000);

   float h = dht.readHumidity();
   float t = dht.readTemperature(); // Celsius is DEFAULT
   float f = dht.readTemperature(true); // Farehnite

   // Check if any reads failed and exit early (to try again).
   if (isnan(h) || isnan(t) || isnan(f))
     {
#ifdef DEBUG
        Serial.println("Failed to read from DHT sensor!");
#endif
        return;
     }

   float hic = dht.computeHeatIndex(t, h, false);

   ThingSpeak.setField(1, h);
   ThingSpeak.setField(2, t);
   ThingSpeak.setField(3, hic);

   ThingSpeak.writeFields(channelNo, myWriteKey);

#ifdef DEBUG
   Serial.println("Humidity:");
   Serial.print(h);
   Serial.println("Temperature:");
   Serial.print(t);
   Serial.print(" C   ");
   Serial.print(f);
   Serial.print("  F  \r\n");


   Serial.print("Heat index:");
   Serial.println(" F");
#endif

   //wait for 30 seconds
   delay(30000);
}

