#include <avr/io.h>
#include <util/delay.h>

//Written by ami 2017
//refer this: https://cdn.sparkfun.com/assets/6/4/7/1/e/51ae0000ce395f645d000000.png

#define delay _delay_ms
#define delayMicroseconds _delay_us

#define sda PB1
#define scl PB0

void sda_low()
{
   DDRB |= (1 << sda);
   PORTB &= ~(1 << sda);
}

void sda_high()
{
   DDRB &= ~(1 << sda);
}

void scl_low()
{
   DDRB |= (1 << scl);
   PORTB &= ~(1 << scl);
}

void scl_high()
{
   DDRB &= ~(1 << scl);
}

uint8_t sda_read()
{
   return bit_is_set(PINB, sda);
}

uint8_t scl_read()
{
   return bit_is_set(PINB, scl);
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
   //now this depends upon totally on i2c slave when it pulls down SCL line
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
   i2c_init();
   uint8_t ack = 1;

   for (uint8_t i = 8; i < 128; ++i)
     {
        ack = start((i << 1) + 1);
        stop();

        if (ack == 0)
          {
             //LED at PB2 is on if there is any i2c slave
             PORTB |= (1 << PB2);
          }
        delay(50);
     }
}

int main(void)
{
   DDRB |= (1 << PB2);

   //wait for 2 seconds
   _delay_ms(2000);

   setup();

   while (1)
     {
     }

   return 0;
}
