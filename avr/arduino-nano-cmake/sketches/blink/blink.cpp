#include <Arduino.h>

#define LED LED_BUILTIN

void setup()
{
   pinMode(LED, OUTPUT);
}

int8_t i = 0;
void loop()
{
   for(i = 0; i < 4; ++i)
     {
        digitalWrite(LED, HIGH);
        delay(i * 100);
        digitalWrite(LED, LOW);
        delay(100 * i);
     }
   delay(1000);
}

