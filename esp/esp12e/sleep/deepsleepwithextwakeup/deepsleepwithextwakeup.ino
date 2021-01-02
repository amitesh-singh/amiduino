/*
 * You can also wake up the ESP8266 with an external wake up, like the press of a button or a reed switch.
 * You just need to put the ESP8266 in deep sleep mode for an indefinite period of time,
 * and then set the RST pin to LOW to wake it up. 
 *
 */

void setup()
{
   Serial.begin(115200);
   Serial.setTimeout(2000);
   //wait for serial to properly initialize
   while (!Serial) {}

   // Deep sleep mode until RESET pin is connected to a LOW signal 
   //basically we need to restart esp8266
   Serial.println("I'm awake, but I'm going to sleep until RESET pin is connected to a low signal");

   ESP.deepSleep(0);
}

void loop()
{
}
