// Smart humidifier 
//(C) Amitesh Singh

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SimpleDHT.h>

#include "index.h"

static void _readDHT11();

static const char ssid[] = "POOJA";
static const char password[] = "60276711";

static const uint8_t led = D4;
static const uint8_t pinDHT11 = D0;
static const uint8_t pinFan = D1;
static const uint8_t pinHeater = D2;

static uint8_t userHumidValue = 35;
static uint8_t humidValue =  0;
static uint8_t roomTemp = 0;
static bool heaterOn = false;
static bool fanOn = false;


static SimpleDHT11 dht11;
static ESP8266WebServer server(80);

static void processHumidityController()
{
   if (humidValue < userHumidValue)
     {
        if (!digitalRead(pinFan))
          {
             Serial.println("Started FAN");
             digitalWrite(pinFan, HIGH);
             fanOn = true;
          }
        if (!digitalRead(pinHeater))
          {
             Serial.println("Started heater:");
             digitalWrite(pinHeater, HIGH);
             heaterOn = true;
          }
     }
   else
     {
        if (digitalRead(pinFan))
          {
             Serial.println("Stopped FAN");
             digitalWrite(pinFan, LOW);
             fanOn = false;
          }
        if (digitalRead(pinHeater))
          {
             Serial.println("Stopped HEATER");
             digitalWrite(pinHeater, LOW);
             heaterOn = false;
          }
     }
}

static void
handleSubmit()
{
   String newHumidVal = server.arg("userHumidValue");
   Serial.println("Set the humid value to:");
   Serial.print(newHumidVal);
   if (newHumidVal.toInt() < 80 || newHumidVal.toInt() > 10)
     {
        userHumidValue = newHumidVal.toInt();
        server.send(200, "text/html", newHumidVal);
     }
   else
     server.send(200, "text/html", "Wrong humidity value");
}

static void
handleRoot()
{
   String s = mainPage;

   if (server.hasArg("userHumidValue"))
     {
        return handleSubmit();
     }

   s.replace("@@humidity@@", String(humidValue));
   s.replace("@@roomTemp@@", String(roomTemp));
   s.replace("@@userhval@@", String(userHumidValue));
   if (fanOn)
     s.replace("@@fan@@", "ON");
   else
     s.replace("@@fan@@", "OFF");
   if (heaterOn)
     s.replace("@@heater@@", "ON");
   else
     s.replace("@@heater@@", "OFF");

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

static volatile bool _forceReadDHT = false;

static void
_timer1_cb()
{
   //Reading sensor value in timer1 - not able to read data.. don;t know why
   //_readDH11();
   _forceReadDHT = true;
}

void setup(void)
{
   pinMode(led, OUTPUT);
   pinMode(pinFan, OUTPUT);
   pinMode(pinHeater, OUTPUT);

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

   server.onNotFound(handleNotFound);

   server.begin();
   Serial.println("HTTP server started");

   _readDHT11();

   //start timer1
   timer1_disable();
   timer1_attachInterrupt(_timer1_cb);
   timer1_isr_init();

   timer1_enable(TIM_DIV265, TIM_EDGE, TIM_LOOP);
   timer1_write(ESP.getCycleCount() + 160e6);
}

void loop(void)
{
   server.handleClient();
   if (_forceReadDHT)
     {
        _readDHT11();
        processHumidityController();
        _forceReadDHT = false;
     }
}

static void
_readDHT11()
{
   digitalWrite(led, !digitalRead(led));

   humidValue = 0;
   roomTemp = 0;
   int err = SimpleDHTErrSuccess;
   if ((err = dht11.read(pinDHT11, &roomTemp, &humidValue, NULL))
       != SimpleDHTErrSuccess)
     {
        Serial.print("Read DHT11 failed, err="); Serial.println(err);
        delay(1000);
        return;
     }

   Serial.print("Humidity: ");
   Serial.print(String(humidValue));
   Serial.print("  Temperature: ");
   Serial.println(String(roomTemp));
}