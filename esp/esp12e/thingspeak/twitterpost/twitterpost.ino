#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti wifimulti;

void setup()
{
   Serial.begin(115200);
   delay(10000);

   Serial.println("");
   Serial.println("");
   Serial.println("");
   Serial.println("");

   wifimulti.addAP("AMIDUINO",
                   "f53*gmPS");
   delay(3000);
}

void loop()
{
   if (wifimulti.run() == WL_CONNECTED)
     {
        HTTPClient http;

        Serial.println("[HTTP] begin...");
        //http.begin("http://52.200.157.52/apps/thingtweet/1/statuses/update");
        http.begin("http://api.thingspeak.com/apps/thingtweet/1/statuses/update");

        int httpCode = http.POST("api_key=LZWJOD0CLKMHKNTJ&status='I just posted this from my thing!'");
        //int httpCode = http.GET();
        delay(10000);
        Serial.println("http return code is");
        Serial.print(httpCode);
        if (httpCode >= 200 && httpCode < 300)
          http.writeToStream(&Serial);
        else
          Serial.println("Failed to post message");
        http.end();

     }

   delay(30000);
}
