#include <ESP8266WiFi.h>

#define SSID "XX"
#define PASSWD "XXX"

static void connectWifi()
{
   WiFi.begin(SSID, PASSWD);
   if (WiFi.status() != WL_CONNECTED)
     {
        Serial.print(".");
        delay(10);
     }
}

static uint8_t ledPin = BUILTIN_LED;
static volatile bool disconnectStatus = false;
WiFiEventHandler disconnectHandler, gotIpHandler;

static void OnDisconnectCb(const WiFiEventStationModeDisconnected& event)
{
   Serial.println("Disconnected.");
   disconnectStatus = true;
   // no need to call following code again, ESP8266
   // does connect automatically.
   //Call following code to reconnect.
   // WiFi.disconnect();
   // delay(1200);
   // connectWifi();
}

static void OnGotIpCb(const WiFiEventStationModeGotIP& event)
{
   Serial.print("Connected, IP:");
   Serial.println(WiFi.localIP());
   disconnectStatus = false;
}

void setup()
{
   pinMode(ledPin, OUTPUT);
   Serial.begin(9600);

   connectWifi();
   Serial.println("");
   Serial.println("Connected.");

   disconnectHandler = WiFi.onStationModeDisconnected(&OnDisconnectCb);

   gotIpHandler = WiFi.onStationModeGotIP(&OnGotIpCb);
}

void loop()
{
   //don't do network related things if there is no WiFi
   if (disconnectStatus) return;

   digitalWrite(ledPin, !digitalRead(ledPin));
   delay(1000);
}
