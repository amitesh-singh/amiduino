
/*
Total 10 Touch button GPIOs
Touch 0 -> GPIO4
Touch 1 -> GPIO0 
Touch 2 -> GPIO2
Touch 3 -> GPIO15
Touch 4 -> GPIO13
Touch 5 -> GPIO12
Touch 6 -> GPIO14
Touch 7 -> GPIO27
Touch 8 -> GPIO33
Touch 9 -> GPIO32

These gpios can be used to wake up esp32 from deep sleep too.

*/
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("ESP32 touch button example");
}

void loop()
{
  Serial.println(touchRead(4)); //Get the value of Touch 0 -> GPIO4
  delay(100);
}