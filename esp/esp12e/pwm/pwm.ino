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

static int dutycycle = 512;
String line;
static uint32_t vcc;

static ESP8266WebServer server(80);
static String vRefTemp;
static String vRef[4] = {"0", "0", "0", "0"};

static void handleSubmit(int pin)
{
   Serial.println("Pwm pin");
   Serial.println(pin);
   if (pin == D1)
    vRefTemp = vRef[0]; //= server.arg("D1userVRef");
   else if (pin == D2)
    vRefTemp = vRef[1]; // = server.arg("D2userVref");
   else if (pin == D3)
    vRefTemp = vRef[2];
   else if (pin == D4)
    vRefTemp = vRef[3];
     
   Serial.println("Set the Vref Value :");
   Serial.print(vRefTemp);
   float humidVal = vRefTemp.toFloat();
   if (humidVal <= 3.3)
     {
        int dutyCycle = (1024/3.3)*humidVal;
        Serial.print("DutyCycle=");
        Serial.println(dutyCycle);
        analogWrite(pin, dutyCycle);
        
        server.send(200, "text/html", vRefTemp);
     }
   else
     server.send(200, "text/html", "Wrong humidity value");
}

static String s;

static void handleRoot()
{
   s = mainPage;

   if (server.hasArg("D1userVRef") && (vRef[0] = server.arg("D1userVRef")) != "")
     {
        return handleSubmit(D1);
     }

    if (server.hasArg("D2userVRef") && (vRef[1] = server.arg("D2userVRef")) != "")
     {
      
        Serial.println("D2 use Vref");
        return handleSubmit(D2);
     }

      if (server.hasArg("D3userVRef") && (vRef[2] = server.arg("D3userVRef")) != "")
     {
      
        Serial.println("D3 use Vref");
        return handleSubmit(D3);
     }
      if (server.hasArg("D4userVRef") && (vRef[3] = server.arg("D4userVRef")) != "")
     {
      
        Serial.println("D4 use Vref");
        return handleSubmit(D4);
     }
  // if (pwmPin == D1) 
    s.replace("@@d1VRef@@", String(vRef[0]));
  // if (pwmPin == D2)
    s.replace("@@d2VRef@@", String(vRef[1]));
    s.replace("@@d3VRef@@", String(vRef[2]));
  // if (pwmPin == D2)
    s.replace("@@d4VRef@@", String(vRef[3]));
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
   pinMode(D2, OUTPUT);
   pinMode(D3, OUTPUT);
   pinMode(D4, OUTPUT);
   
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
