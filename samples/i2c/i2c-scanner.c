
// I2C Scanner - Master

#include <Wire.h>

//find the list of i2c devices addresses
byte count = 0;

void setup()
{
  Serial.begin(9600);

  Wire.begin(); // join i2c bus (address optional for master)

  Serial.println("I2c scanner.... starts....");
  // only 1 to 119 are allowed, 0 is for broadcast
  for(byte addr = 1; addr < 120; ++addr)
  {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0)
    {

      Serial.println("--------");
      Serial.println("Found a device");
      Serial.print(addr, DEC);
      Serial.println("");
      Serial.print("0x");
      Serial.print(addr, HEX);
      Serial.println("");
      Serial.println("###########");

      ++count;
      delay(1);
    }
  }
  Serial.println("Done..");
  Serial.print("Found ");
  Serial.print(count, DEC);
  Serial.println(" Device(s)");
}

void loop()
{
}
