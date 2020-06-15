//use nodemcu 1.0 (ESP12E board)
// PWM dac code sample code
#include "config.h"
#include "index.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define DAC D1
#define LED D4
ADC_MODE(ADC_VCC);

static int dutycycle = 512;
String line;
static uint32_t vcc;

static ESP8266WebServer server(80);

static void handleSubmit()
{
   String newHumidVal = server.arg("userVRef");

   Serial.println("Set the Vref Value :");
   Serial.print(newHumidVal);
   float humidVal = newHumidVal.toFloat();
   if (humidVal <= 3.3)
     {
        int dutyCycle = (1024/3.3)*humidVal;
        Serial.println("DutyCycle=");
        Serial.println(dutyCycle);
        analogWrite(DAC, dutyCycle);
        server.send(200, "text/html", newHumidVal);
     }
   else
     server.send(200, "text/html", "Wrong humidity value");
}

static String s;

static void handleRoot()
{
  
   s = mainPage;

   if (server.hasArg("userVRef"))
     {
        return handleSubmit();
     }
 
 
   server.send(200, "text/html", s);
}
static void handleNotFound();

void setup()
{
   pinMode(LED, OUTPUT);
   digitalWrite(LED, 0); // LED is ON


   
   
   Serial.begin(115200);
   //50% duty cycle
   pinMode(DAC, OUTPUT);
   Serial.println("Starting....");

   //fix reconnect issue.
   WiFi.mode(WIFI_OFF);
   WiFi.mode(WIFI_STA);
   WiFi.setOutputPower(0);
   WiFi.begin(ssid, password);

   Serial.println("");

   Serial.print("Connected to ");
   Serial.println(ssid);
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());

   //can access via humid.local
   if (MDNS.begin("humid"))
     {
        Serial.println("MDNS responder started");
     }

   server.on("/", handleRoot);

   server.onNotFound(handleNotFound);

   server.begin();
   Serial.println("HTTP server started");


   //default frequency is 1Khz
   /*
    * PWM frequency is 1kHz by default. Call analogWriteFreq(new_frequency) to change the frequency. Valid values are from 100Hz up to 40000Hz.
    */
   analogWriteFreq(20000);


  // analogWrite(DAC, dutycycle);
}

void loop()
{
   server.handleClient();
}

static void handleNotFound()
{
  // digitalWrite(led, 1);
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
  // digitalWrite(led, 0);
}
