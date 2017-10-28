/*
 * Smart Humidifier
 * Copyright (C) 2017  Amitesh Singh <singh.amitesh@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SimpleDHT.h>

#include <EEPROM.h>

#include "index.h"
#include "config.h"

static void _readDHT11();

static uint8_t userHumidValue = 35;
static uint8_t humidValue =  0;
static uint8_t roomTemp = 0;
static bool heaterOn = false;
static bool fanOn = false;

//static SimpleDHT11 dht11;
static SimpleDHT22 dht22;

static ESP8266WebServer server(80);
static volatile bool _forceReadDHT = false;

static void processHumidityController()
{
   if (humidValue < userHumidValue)
     {
        if (digitalRead(pinFan))
          {
             Serial.println("Started FAN");
             digitalWrite(pinFan, LOW);
             fanOn = true;
          }
        if (digitalRead(pinHeater))
          {
             Serial.println("Started heater:");
             digitalWrite(pinHeater, LOW);
             heaterOn = true;
          }
     }
   else
     {
        if (!digitalRead(pinFan))
          {
             Serial.println("Stopped FAN");
             digitalWrite(pinFan, HIGH);
             fanOn = false;
          }
        if (!digitalRead(pinHeater))
          {
             Serial.println("Stopped HEATER");
             digitalWrite(pinHeater, HIGH);
             heaterOn = false;
          }
     }
}

static void handleSubmit()
{
   String newHumidVal = server.arg("userHumidValue");

   Serial.println("Set the humid value to:");
   Serial.print(newHumidVal);

   if ((newHumidVal.toInt() != userHumidValue)
       && (newHumidVal.toInt() < 80 || newHumidVal.toInt() > 10))
     {
        userHumidValue = newHumidVal.toInt();
        EEPROM.write(0, userHumidValue);
        EEPROM.commit();
        server.send(200, "text/html", newHumidVal);
        processHumidityController();
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

   s.replace("@@humidity@@", String(humidValue));
   s.replace("@@roomTemp@@", String(roomTemp));
   s.replace("@@userhval@@", String(userHumidValue));
   if (fanOn || heaterOn)
     s.replace("@@color@@", "red");
   else
     s.replace("@@color@@", "black");

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

static void handleNotFound()
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

static void _timer1_cb()
{
   //Reading sensor value in timer1 - not able to read data.. don;t know why
   //_readDH11();
   _forceReadDHT = true;
}

static void _blinkLed(uint8_t n)
{
  uint8_t i = 0;

  for (; i < 2 * n; ++i)
  {
    digitalWrite(led, !digitalRead(led)); //toggle
    delay(500);
  }
}

void setup(void)
{
   pinMode(led, OUTPUT);
   pinMode(pinFan, OUTPUT);
   pinMode(pinHeater, OUTPUT);

   digitalWrite(pinFan, HIGH); //high is OFF here
   digitalWrite(pinHeater, HIGH); //high is OFF
   
   digitalWrite(led, HIGH); // led is OFF
   
   //on starting up, toggle for 4 times
   _blinkLed(4);

   Serial.begin(115200);
   //start the eeprom
   EEPROM.begin(512);
   digitalWrite(led, 0);

   // Read eeprom value at address 0 from EEPROM
   userHumidValue = EEPROM.read(0);
   if (userHumidValue == 0xFF)
     {
        userHumidValue = 35;
        Serial.print("Failed to read EEPROM - first time setup?");
     }

  //fix reconnect issue.
   WiFi.mode(WIFI_OFF);
   WiFi.mode(WIFI_STA);
   WiFi.setOutputPower(0);
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
   _blinkLed(4); //blink 4 times on wifi connected
   //can access via humid.local
   if (MDNS.begin("humid"))
     {
        Serial.println("MDNS responder started");
     }

   server.on("/", handleRoot);

   server.onNotFound(handleNotFound);

   server.begin();
   Serial.println("HTTP server started");

   _readDHT22();

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
        _readDHT22();
        processHumidityController();
        _forceReadDHT = false;
     }
}

static void _readDHT22()
{
   int err = SimpleDHTErrSuccess;

   digitalWrite(led, !digitalRead(led));

   humidValue = 0;
   roomTemp = 0;
   if ((err = dht22.read(pinDHT11, &roomTemp, &humidValue, NULL))
       != SimpleDHTErrSuccess)
     {
        Serial.print("Read DHT22 failed, err="); Serial.println(err);
        delay(1000);
        return;
     }

   Serial.print("Humidity: ");
   Serial.print(String(humidValue));
   Serial.print("  Temperature: ");
   Serial.println(String(roomTemp));
}
