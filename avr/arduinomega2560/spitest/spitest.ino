#include <Arduino.h>
#include <SPI.h>

void setup()
{
   pinMode(MOSI, OUTPUT);
   pinMode(MISO, INPUT);
   Serial.begin(115200);
   SPI.begin();
   //connect MISO - 50 and MOSI - 51 together
}

void loop()
{
   uint8_t val;

   val = SPI.transfer(10);
   Serial.println(val);

   delay(1000);
}
