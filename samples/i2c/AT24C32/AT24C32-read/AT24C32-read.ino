#include <Wire.h>

#define AT24C32_I2C_ADDR 0x57

void setup()
{
   Serial.begin(9600);

   Wire.begin();
   Serial.println("Going to read AT24C32 EEPROM present on DS3231 module.");
   delay(5000);
   Serial.println("Going to read EEPROM");

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

   //lets read some addr of eeprom
   Wire.beginTransmission(AT24C32_I2C_ADDR);
   uint16_t addr = 0;
   Wire.write(addr >> 8); //Address MSB
   Wire.write(addr & 0xFF); // Address LSB
   Wire.endTransmission();

   //lets read 8 bytes or so
   Wire.requestFrom(AT24C32_I2C_ADDR, 8);
   uint8_t i = 0;
   unsigned char buf[9];
   for (; i < 8 && Wire.available(); ++i)
     {
        buf[i] = Wire.read();
        Serial.print(buf[i], HEX);
        Serial.print(":");
     }
   Serial.println("");
   //buf[8] = 0;
   //char *str = buf;

   //Serial.println(str);
   pinMode(13, OUTPUT);
}

void loop()
{
   delay(1000);
   digitalWrite(13, 1);
   delay(1000);
   digitalWrite(13, 0);
}
