#include <Wire.h>

#define AT24C32_I2C_ADDR 0x57

void setup()
{
   Serial.begin(9600);

   Wire.begin();
   Serial.println("Going to read AT24C32 EEPROM present on DS3231 module.");
   delay(5000);
   Serial.println("Going to read EEPROM");
   uint16_t addr = 0;

   //check if device is present
   Wire.beginTransmission(AT24C32_I2C_ADDR);
   if (Wire.endTransmission() == 0)
     {
        Serial.println("AT24C32 addr: 0x57 is present on I2C line");
     }
   else
     {
        Serial.println("AT24C32 is not present.. aborting..");
        exit(0);
        return;
     }

   // Lets write a byte to eeprom

   Wire.beginTransmission(AT24C32_I2C_ADDR);
   Wire.write(addr >> 8);
   Wire.write(addr);
   Wire.write('A'); //Write an A
   //Wire.write('B'); //you write at second address.
   delay(10);
   Wire.endTransmission();
   delay(10);

   //lets read some addr of eeprom
   Wire.beginTransmission(AT24C32_I2C_ADDR);
   Wire.write(addr >> 8); //Address MSB
   Wire.write(addr & 0xFF); // Address LSB
   Wire.endTransmission();

   //lets read 1 bytes and print it
   Wire.requestFrom(AT24C32_I2C_ADDR, 1);
   delay(10);
   Serial.println("Read 1 byte:");
   Serial.print(Wire.read(), HEX);

   pinMode(13, OUTPUT);
}

void loop()
{
   delay(1000);
   digitalWrite(13, 1);
   delay(1000);
   digitalWrite(13, 0);
}
