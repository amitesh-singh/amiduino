#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "AMIDUINO";
const char* password = "your password";

const char* newssid = "EspTest1";
const char* newpassword = "12345";

const char* host = "data.sparkfun.com";

ESP8266WebServer server(80);

int count = 0;

void setup(void)
{
   Serial.begin(115200);
   Serial.println("");

   // set both access point and station
   WiFi.mode(WIFI_AP_STA);

   WiFi.softAP(newssid, newpassword);

   Serial.print(newssid);
   Serial.print(" server ip: ");
   Serial.println(WiFi.softAPIP());

   server.on("/", handleRoot); 
   server.onNotFound(handleNotFound); 
   server.begin();
   Serial.println("HTTP server started");

   if (strcmp (WiFi.SSID().c_str(),ssid) != 0)
     {
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.begin(ssid, password);
     }

   while (WiFi.status() != WL_CONNECTED)
     {
        yield();
     }

   Serial.print("Connected to: ");
   Serial.print(WiFi.SSID());
   Serial.print(", IP address: ");
   Serial.println(WiFi.localIP());
}

void loop(void)
{
   server.handleClient();
}

void handleRoot()
{
   Serial.print("handleRoot: ");
   Serial.println(count);
   String s = "request count: ";
   s += ++count;
   server.send(200, "text/plain", s);
}

void handleNotFound()
{
   Serial.print("proxy request to ");
   Serial.println(host);

   count++;

   WiFiClient client;
   while (!!!client.connect(host, 80)) {
        Serial.println("connection failed, retrying...");
   }

   Serial.print("Requesting uri: ");
   String requestUri = server.uri();

   // TODO: an easier way to get the request url?
   if (server.args() > 0)
     {
        requestUri += "?";
        for (int i=0; i<server.args(); i++)
          {
             requestUri += server.argName(i);
             requestUri += "=";
             requestUri += server.arg(i);
             if (i+1<server.args())
               {
                  requestUri += "&";
               }
          }
     }
   Serial.println(requestUri);

   client.print(String("GET ") + requestUri);

   client.print(String(" HTTP/1.1\r\n") +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");

   while(!!!client.available())
     {
        yield();
     }

   String response = "";
   while(client.available())
     {
        response += client.read();
     }
   server.send(200, "text/html", response);

   client.stop();
}
