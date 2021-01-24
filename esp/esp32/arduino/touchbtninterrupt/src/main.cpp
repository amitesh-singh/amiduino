#include <Arduino.h>

static unsigned int calltimes = 0;

void callback()
{
  calltimes++;
  Serial.print("GPIO4/T0 is touched ");
  Serial.println(calltimes);
  Serial.println("");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("touch using interrupt");

  touchAttachInterrupt(T0, callback, 30);
}

void loop() 
{
  
}