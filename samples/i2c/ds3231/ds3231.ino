
#include <Wire.h>

//Read the date/time from DS3231 module
// You could also use DS3231 library, far easier. but wire lib is fun

// My DS3231 module have  AT24C32 (storage capacity 32K, EEPROM) also its address is 0x57. so you can have
// fun with it also.
#define DS3231_I2C_ADDR 0x68
#define AT24C32_I2C_ADDR 0x57 //i2c scanner shows its presense too.

// Convert normal decimal numbers to binary coded decimal
static byte decToBcd(byte val)
{
   return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
static byte bcdToDec(byte val)
{
   return( (val/16*10) + (val%16) );
}

static void setTime(byte seconds, byte minutes, byte hours, byte dayofweek, byte dayofmonth, byte month, byte year)
{
   Wire.beginTransmission(DS3231_I2C_ADDR);

   Wire.write(0);
   Wire.write(decToBcd(seconds));
   Wire.write(decToBcd(minutes));
   Wire.write(decToBcd(hours));
   Wire.write(decToBcd(dayofweek));
   Wire.write(decToBcd(dayofmonth)); // 1 - 31
   Wire.write(decToBcd(month)); // (1 - 12)
   Wire.write(decToBcd(year)); // 0 - 99

   Wire.endTransmission();
}

static void readTime()
{
   byte value;

   Wire.beginTransmission(DS3231_I2C_ADDR);
   Wire.write(0);
   Wire.endTransmission();

   //request for 7 bytes
   Wire.requestFrom(DS3231_I2C_ADDR, 7);

   value = Wire.read(); //read seconds
   Serial.print("second:");
   Serial.println(bcdToDec(value & 0x7f));

   value = Wire.read(); // read minutes
   Serial.print("minutes:");
   Serial.println(bcdToDec(value));

   value = Wire.read(); //read hours
   Serial.print("hours:");
   Serial.println(bcdToDec(value & 0x3f));

   value = Wire.read(); //read dayofweek
   Serial.print("day of week:");
   Serial.println(bcdToDec(value));

   value = Wire.read(); //read dayofmonth
   Serial.print("day of month:");
   Serial.println(bcdToDec(value));

   value = Wire.read(); //read month
   Serial.print("month:");
   Serial.println(bcdToDec(value));

   value = Wire.read(); //read year
   Serial.print("year:");
   Serial.println(bcdToDec(value));
}

void setup()
{
   Serial.begin(9600);

   Wire.begin();
}

void loop()
{
   delay(1000);
   readTime();
}
