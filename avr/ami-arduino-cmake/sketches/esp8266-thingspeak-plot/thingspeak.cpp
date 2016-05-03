#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdlib.h>

//Circuit
// https://123d.circuits.io/circuits/2083460-esp8266-thing-speak
//use LCD for debugging
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int errorLED = 11;

String ssid                      = "Simulator Wifi"; // SSID to connect to
String password                  = ""; // Our virtual wifi has no password (so dont do your banking stuff on this network)

String host                      = "api.thingspeak.com"; // Thingspeak API
const int httpPort   = 80;
String uri      = "/update?api_key=BHYXFOGPKH8J73JT&field1=";

// the setup routine runs once when you press reset:
void setup()
{
   // Setup LCD and put some information text on there
   lcd.begin(16,2);
   lcd.print("LM35 Logger:");
   lcd.setCursor(0,1);
   lcd.print("Start....");

   pinMode(errorLED, OUTPUT); // init our red error LED

   // Start our ESP8266 Serial Communication
   Serial.begin(115200);    // Serial connection over USB to computer
   Serial.println("AT");    // Serial connection on Tx / Rx port to ESP8266
   delay(10);            // Wait a little for the ESP to respond
   if (!Serial.find("OK")) digitalWrite(errorLED, HIGH);  // check if the ESP is running well

   // Connect to 123D Circuits Simulator Wifi
   Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
   delay(10);            // Wait a little for the ESP to respond
   if (!Serial.find("OK"))
     digitalWrite(errorLED, HIGH);  // check if the ESP is running well

   //For Tcp client, AT+CIPMUX=0;
   Serial.println("AT+CIPMUX=0");
   delay(50);

   // Open TCP connection to the host:
   Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
   delay(50);            // Wait a little for the ESP to respond
   if (!Serial.find("OK"))
     digitalWrite(errorLED, HIGH);  // check if the ESP is running well
   else
     {
        lcd.setCursor(3, 1);
        lcd.print("conn ok thingspeak");
        delay(4000);
     }
}

char outstr[4];

void loop()
{
   //Read the LM35 connected at Port A0 of Arduino
   float fVal = analogRead(A0);
   delay(10);

   //This is used for converting float to string
   // PRecision for decimal is set to 0 since thingspeak does not like it? TODO: check the root cause
   dtostrf(fVal, 3, 0, outstr);
   String httpPacket = "GET " + uri;
   lcd.setCursor(0, 1);
   lcd.print(outstr);
   delay(5000);
   httpPacket += outstr; // Temperature reading
   httpPacket += " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";

   int length = httpPacket.length();

   // Send our message length
   Serial.print("AT+CIPSEND=");
   Serial.println(length);
   delay(10); // Wait a little for the ESP to respond
   if (!Serial.find(">"))
     digitalWrite(errorLED, HIGH); // check if the ESP is running well

   // Send our http request
   Serial.print(httpPacket);
   delay(10); // Wait a little for the ESP to respond
   if (!Serial.find("SEND OK\r\n"))
     {
        digitalWrite(errorLED, HIGH); // check if the ESP is running well
     }
   else
     {
        //TODO: write a lcd library for logger 
        lcd.setCursor(3, 0);
        lcd.print("DataSentSuccess");
     }

// Following command closes the current TCP connection
   //Serial.println("AT+CIPCLOSE");
//NOTE: After closing a connection, not able to reconnect on 123dcircuit. it is probably disabled because of Flooding threats
   delay(30000);
}

