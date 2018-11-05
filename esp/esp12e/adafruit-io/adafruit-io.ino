// refer this guide.
// https://www.instructables.com/id/Control-Your-Projects-With-Google-Assistant-and-Ad/

#include <ESP8266WiFi.h>

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define SSID  "AndroidHotspot4095"
#define password ""

#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT 1883

#define MQTT_USER "userid"
#define MQTT_PASS "key.."

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASS);

Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USER "/feeds/onoff");

void MQTT_connect()
{
   int8_t ret;
   // Stop if already connected
   if (mqtt.connected())
     {
        return;
     }

   Serial.print("Connecting to MQTT... ");
   uint8_t retries = 3;
   while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
     {
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);  // wait 5 seconds
        retries--;
        if (retries == 0)
          {
             // basically die and wait for WDT to reset me
             while (1);
          }
     }
   Serial.println("MQTT Connected!");
}

void setup()
{
   Serial.begin(9600);

   Serial.println("Connecting to WIFI");

   WiFi.begin(SSID, password);
   if (WiFi.status() != WL_CONNECTED)
     {
        Serial.print(".");
        delay(10);
     }

   Serial.print("Connected!!");
   //subscribe to onoff topic 
   mqtt.subscribe(&onoff);

   pinMode(BUILTIN_LED, OUTPUT);
   delay(1000);
   digitalWrite(BUILTIN_LED, HIGH);
   delay(1000);
   digitalWrite(BUILTIN_LED, LOW);
}

void loop()
{
   MQTT_connect();
   //Read from our subscription queue until we run out, or
   //wait up to 5 seconds for subscription to update
   Adafruit_MQTT_Subscribe *subscribe;

   while ((subscribe = mqtt.readSubscription(5000)))
     {
        if (subscribe == &onoff)
          {
             Serial.print("onoff: ");
             Serial.println((char *)onoff.lastread);

             if (!strcmp((char *)onoff.lastread, "ON"))
               {
                  digitalWrite(BUILTIN_LED, LOW);
               }
             else
               digitalWrite(BUILTIN_LED, HIGH);
          }
     }

   if (!mqtt.ping())
     mqtt.disconnect();
}

