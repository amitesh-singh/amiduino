//Written by ami 2017
//refer this: https://cdn.sparkfun.com/assets/6/4/7/1/e/51ae0000ce395f645d000000.png

#define sda 9
#define scl 8

void sda_low()
{
   pinMode(sda, OUTPUT);
   digitalWrite(sda, LOW);
}

void sda_high()
{
   pinMode(sda, INPUT);
}

void scl_low()
{
   pinMode(scl, OUTPUT);
   digitalWrite(scl, LOW);
}

void scl_high()
{
   pinMode(scl, INPUT);
}

uint8_t sda_read()
{
   return digitalRead(sda);
}

uint8_t scl_read()
{
   return digitalRead(scl);
}

void stop()
{
   scl_low();
   delayMicroseconds(10);

   sda_low();
   delayMicroseconds(10);

   scl_high();
   delayMicroseconds(10);

   sda_high();
   delayMicroseconds(10);
}

void i2c_init()
{
   stop();
}

uint8_t write(uint8_t data)
{
   for(uint8_t i = 8; i; --i)
     {
        scl_low();
        if(data & 0x80)
          {
             sda_high();
          }
        else
          {
             sda_low();
          }
        delayMicroseconds(10);

        scl_high();

        delayMicroseconds(10);

        data <<= 1;
     }

   //get ACK
   scl_low();

   sda_high();
   delayMicroseconds(10);

   scl_high();

   //wait scl to go low? - controlled by slave?
   delayMicroseconds(10);
   //delay(10);
   //scl_low();

   return sda_read();
}

uint8_t start(uint8_t rawAddr)
{
   sda_low();
   delayMicroseconds(10);

   scl_low();
   delayMicroseconds(10);

   return write(rawAddr);
}

void setup()
{
   Serial.begin(9600);

   i2c_init();
   uint8_t ack = 1;

   Serial.println("scanning..");
   for (uint8_t i = 8; i < 128; ++i)
     {
        ack = start((i << 1) + 1);
        stop();

        if (ack == 0)
          {
             Serial.println("found i2c device at address: ");
             Serial.print(i);
             Serial.print(", 0x");
             Serial.print(i, HEX);
             Serial.println("");
          }
        delay(50);
     }

   Serial.println("scanning done");
}

void loop()
{
}
