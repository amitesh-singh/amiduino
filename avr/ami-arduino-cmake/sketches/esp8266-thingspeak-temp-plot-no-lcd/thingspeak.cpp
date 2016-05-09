#include <Arduino.h>
#include <stdlib.h>

//Circuit

int errorLED = 13;

static const String ssid                      = ""; // SSID to connect to
static const String password                  = ""; // wifi password

static const String host                      = "api.thingspeak.com"; // Thingspeak API
static const int httpPort                     = 80;
static const String uri                       = "/update?api_key=BHYXFOGPKH8J73JT&field1=";

static void showBlink(int n)
{
   for (int i = 0; i < n; ++i)
     {
        digitalWrite(errorLED, HIGH);
        delay(1000);
        digitalWrite(errorLED, LOW);
        delay(1000);
     }

   delay(3000);
   digitalWrite(errorLED, HIGH);
}

// the setup routine runs once when you press reset:
void setup()
{
   //wait 3 seconds
   delay(3000);
   // Setup LCD and put some information text on there
   pinMode(errorLED, OUTPUT); // init our red error LED

   // Start our ESP8266 Serial Communication
   Serial.begin(115200);    // Serial connection over USB to computer
   Serial.println("AT");    // Serial connection on Tx / Rx port to ESP8266
   delay(1000);            // Wait a little for the ESP to respond
   if (!Serial.find("OK"))
     {
        showBlink(1);
     }

   Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
   delay(1000);            // Wait a little for the ESP to respond
   if (!Serial.find("OK"))
     {
        showBlink(2);
     }

   //For Tcp client, AT+CIPMUX=0;
   Serial.println("AT+CIPMUX=0");
   delay(1000);
}

static char outstr[4];

void loop()
{
   //Read the LM35 connected at Port A0 of Arduino
   float fVal = (500.0 * analogRead(A0))/1024.0;
   delay(1000);

   //For Tcp client, AT+CIPMUX=0;
   Serial.println("AT+CIPMUX=0");
   delay(1000);

   // Open TCP connection to the host:
   Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
   delay(1000);            // Wait a little for the ESP to respond
   if (!Serial.find("OK"))
     {
        showBlink(4);
     }
   else
     {
        //wait for 4 seconds
        delay(5000);
     }
   //This is used for converting float to string
   // PRecision for decimal is set to 0 since thingspeak does not like it? TODO: check the root cause
   dtostrf(fVal, 3, 0, outstr);
   String httpPacket = "GET " + uri;
   httpPacket += outstr; // Temperature reading
   httpPacket += " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";

   int length = httpPacket.length();

   // Send our message length
   Serial.print("AT+CIPSEND=");
   Serial.println(length);
   delay(1000); // Wait a little for the ESP to respond
   if (!Serial.find(">"))
     {
        showBlink(5);
     }

   // Send our http request
   Serial.print(httpPacket);
   delay(2000); // Wait a little for the ESP to respond
   if (!Serial.find("SEND OK\r\n"))
     {
        showBlink(6);
     }

   // Following command closes the current TCP connection
   Serial.println("AT+CIPCLOSE");
   //wait for 30 seconds
   delay(60000);
}
