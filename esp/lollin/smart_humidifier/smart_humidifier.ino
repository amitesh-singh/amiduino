#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

static const char ssid[] = "POOJA";
static const char password[] = "60276711";
const char mainPage[] PROGMEM = "<html> \
<head> \
<meta http-equiv='refresh' content='3'> \
<title>Smart Humidifier by Ami</title> \
<style> \
div.card { \
  width: 250px; \
  box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19); \
  text-align: center; \
  border-radius: 5px; \
  background-color: #F5F7A0 \
} \
div.header { \
    background-color: #E03C3C; \
    color: white; \
    padding: 10px; \
    font-size: 40px; \
    border-radius: 5px; \
} \
div.container { \
    padding: 4px; \ 
} \
</style> \
</head> \
<body> \
<center><h1> Smart Humidifier </h1> \
<form> \
Set Humid Value (0-80): \
<input type='text' name='userHumidValue' maxlength='2' size='2'> \
<input type='submit' value='Set Humid'> <br> <br> \
</form> \
<div class='card'> \
<div class='header'> \
  <h2>@@oldhvalue@@ C</h2> \
</div> \
<div class='container'> \
  <h3>Humidity</h3> \
</div> \
</div> \
<div class='card'> \
  <div class='header'> \
    <h2>@@roomTemp@@ C</h2> \
  </div> \
  <div class='container'> \
    <h3>Temperature</h3> \
  </div> \
</div> \
</center> \
</body> \
</html>";


static ESP8266WebServer server(80);

static const uint8_t led = D4;

static uint8_t userHumidValue = 0;
static uint8_t humidValue =  45;
static uint8_t roomTemp = 25;

static void handleSubmit()
{
  String newHumidVal = server.arg("userHumidValue");
  Serial.println("Set the humid value to:");
  Serial.print(newHumidVal);
  if (newHumidVal.toInt() > 80 || newHumidVal.toInt() < 20)
  {
    humidValue = newHumidVal.toInt();
    server.send(200, "text/html", newHumidVal);
  }
  else 
    server.send(200, "text/html", "Wrong humidity value");
}
static void handleRoot()
{
   String s = mainPage;

   if (server.hasArg("userHumidValue"))
   {
     return handleSubmit();
   }
   s.replace("@@oldhvalue@@", String(humidValue));
   s.replace("@@roomTemp@@", String(roomTemp));

   server.send(200, "text/html", s);
}

void
handleNotFound()
{
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
   //server.on("/LEDOFF", handleLEDOFF);
   //server.on("/LEDON", handleLEDON);
   //server.on("")
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
