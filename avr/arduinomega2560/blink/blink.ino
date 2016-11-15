#include <Arduino.h>

void setup()
{
   pinMode(13, OUTPUT);
   //the serial output goes to /dev/ttyACM0 - original mega i have
   Serial.begin(115200);
}

void loop()
{
   digitalWrite(13, HIGH);
   Serial.println("Port high");
   delay(1000);
   digitalWrite(13, LOW);
   Serial.println("Port Low");sudo
   delay(1000);
}
