#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

 const char* ssid = "POOJA";
 const char* password = "60276711";

ESP8266WebServer server(80);

const int led = 2;

void handleRoot()
{
   char temp[400];

   sprintf(temp, "%s", "<html> \
   <head></head> \
   <body> \
   <form type='GET' action='LEDON'> \
   <button type='submit'>LED ON</button> \
   </form> \
   <form type='GET' action='LEDOFF'> \
   <button type='submit'>LED OFF</button> \
   </form> \
   </body> \
   </html>");

   server.send(200, "text/html", temp);
}

void handleLEDON()
{
   digitalWrite(led, 1);
   server.send(200, "text/plain", "LED is ON!");
}

void handleLEDOFF()
{
   digitalWrite(led, 0);
   server.send(200, "text/plain", "LED is OFF!");
}

void handleNotFound(){
     digitalWrite(led, 1);
     String message = "File Not Found\n\n";
     message += "URI: ";
     message += server.uri();
     message += "\nMethod: ";
     message += (server.method() == HTTP_GET)?"GET":"POST";
     message += "\nArguments: ";
     message += server.args();
     message += "\n";
     for (uint8_t i=0; i<server.args(); i++){
          message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
     }
     server.send(404, "text/plain", message);
     digitalWrite(led, 0);
}

void setup(void)
{
   pinMode(led, OUTPUT);
   digitalWrite(led, 0);
   Serial.begin(115200);
   WiFi.begin(ssid, password);
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
   server.on("/LEDOFF", handleLEDOFF);
   server.on("/LEDON", handleLEDON);

   server.on("/inline", [](){
             server.send(200, "text/plain", "this works as well");
             });

   server.onNotFound(handleNotFound);

   server.begin();
   Serial.println("HTTP server started");
}

void loop(void)
{
   server.handleClient();
}
