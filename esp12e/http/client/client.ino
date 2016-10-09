#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WifiMulti;

void setup()
{
   Serial.begin(115200);
   Serial.println();
   Serial.println();
   Serial.println();

   for (uint8_t t = 4; t > -0; --t)
     {
        Serial.printf("[Setup] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
     }

   //This makes connection with your wifi router 
   WifiMulti.addAP("AMIDUINO", "wifi password");
}

void loop()
{
   if (WifiMulti.run() == WL_CONNECTED)
     {
        HTTPClient http;
        // for cherrypy code,
        // use http://ip/index for def index()
        // for port other than 8080
        // http.begin("192.168.0.101", 8080, "/index");
        // note: http://192.168.0.101/" won;t work. use http://ip/index instead.
        http.begin("http://192.168.0.101/index"); // use HTTP
        delay(100);

        int httpCode = http.GET();
        delay(100);
        if (httpCode > 0)
          {
             if (httpCode == HTTP_CODE_OK)
               {
                  String payload = http.getString();
                  Serial.println(payload);
               }
               String payload = http.getString();
               Serial.println(payload);
               
          }
        else
          Serial.println("[HTTP] GET .. failed");

  String payload = http.getString();
               Serial.println(payload);
         Serial.println("Closing connection");

        http.end();
     }

    Serial.println("Will try reconnecting in 5 seconds");

   delay(5000);
}
