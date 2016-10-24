// This is an example of how to run json based esp8266 server

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//include this header
#include <ArduinoJson.h>

const char* ssid = "AMIDUINO";
const char* password = "";

ESP8266WebServer server(80);

static JsonObject& prepareResponse(JsonBuffer& jsonBuffer)
{
   JsonObject& root = jsonBuffer.createObject();

   JsonArray& analogValues = root.createNestedArray("analog");
   for (int pin = 0; pin < 6; pin++)
     {
        int value = pin;
        analogValues.add(value);
     }

   JsonArray& digitalValues = root.createNestedArray("digital");
   for (int pin = 0; pin < 14; pin++)
     {
        int value = pin;
        digitalValues.add(value);
     }
   return root;
}

void handleRoot()
{
   char temp[400];
   StaticJsonBuffer<400> jsonBuffer;
   JsonObject& json = prepareResponse(jsonBuffer);

   json.printTo(temp, 400);
   server.send(200, "application/json", temp);
   json.printTo(Serial);
}

void handleNotFound()
{
   String message = "File Not Found\n\n";
   message += "URI: ";
   message += server.uri();
   message += "\nMethod: ";
   message += (server.method() == HTTP_GET)?"GET":"POST";
   message += "\nArguments: ";
   message += server.args();
   message += "\n";
   for (uint8_t i=0; i<server.args(); i++)
     {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
     }
   server.send(404, "text/plain", message);
}

void setup(void)
{
   Serial.begin(115200);
   delay(100);
   WiFi.begin(ssid, password);
   Serial.println("");
   Serial.println("");
   Serial.println("");

   // Wait for connection
   while (WiFi.status() != WL_CONNECTED)
     {
        delay(500);
        Serial.print(".");
     }
   Serial.println("");
   Serial.print("Connected to ");
   Serial.println(ssid);
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());

   if (MDNS.begin("esp8266"))
     {
        Serial.println("MDNS responder started");
     }

   server.on("/", handleRoot);
   server.onNotFound(handleNotFound);

   server.begin();
   Serial.println("HTTP server started");
}

void loop(void)
{
   server.handleClient();
}
