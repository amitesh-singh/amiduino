#include "DHT.h"

#define DHTPIN 4

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
   Serial.begin(115200);
   while (!Serial)
   {
     //wait for serial to get ready or else you get crash, especially on esp12
   }

   Serial.println("DHT11 test..");
   dht.begin();
}

void loop()
{
   delay(2000);

   float h = dht.readHumidity();
   float t = dht.readTemperature(); // Celsius is DEFAULT
   float f = dht.readTemperature(true); // Farehnite

   // Check if any reads failed and exit early (to try again).
   if (isnan(h) || isnan(t) || isnan(f))
     {
        Serial.println("Failed to read from DHT sensor!");
        return;
     }
   Serial.println("Humidity:");
   Serial.print(h);
   Serial.println("Temperature:");
   Serial.print(t);
   Serial.print(" C   ");
   Serial.print(f);
   Serial.print("  F  \r\n");

   // Compute heat index in Fahrenheit (the default)
   float hif = dht.computeHeatIndex(f, h);
   // Compute heat index in Celsius (isFahreheit = false)
   float hic = dht.computeHeatIndex(t, h, false);

   Serial.print("Heat index:");
   Serial.print(hif);
   Serial.print(" C   ");
   Serial.print(hic);
   Serial.println(" F");
}

