#include <avr/io.h>
#include <util/delay.h>

#include "i2csoft.h"
//Written by ami 2017
//refer this: https://cdn.sparkfun.com/assets/6/4/7/1/e/51ae0000ce395f645d000000.png

#define delay _delay_ms
#define delayMicroseconds _delay_us

#define sda PB1
#define scl PB0

static void sda_low()
{
   DDRB |= (1 << sda);
   PORTB &= ~(1 << sda);
}

static void sda_high()
{
   DDRB &= ~(1 << sda);
}

static void scl_low()
{
   DDRB |= (1 << scl);
   PORTB &= ~(1 << scl);
}

static void scl_high()
{
   DDRB &= ~(1 << scl);
}

static uint8_t sda_read()
{
   return bit_is_set(PINB, sda);
}

static uint8_t scl_read()
{
   return bit_is_set(PINB, scl);
}

void i2c_begin()
{
   i2c_stop();
}

uint8_t i2c_start(uint8_t rawAddr)
{
   sda_low();
   delayMicroseconds(10);

   scl_low();
   delayMicroseconds(10);

   return i2c_writeByte(rawAddr);
}

void i2c_stop()
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

uint8_t i2c_writeByte(uint8_t data)
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

   //now this depends upon totally on i2c slave when it pulls down SCL line
   // lets wait for i2c slave to pull down scl low and  then we read the sda
  // while (scl_read())
  // {
     //note: if no i2c slave is connected, it would infinitely wait here.. so better to have a timeout here
   //  __asm__ volatile ("nop");
   //}
   delayMicroseconds(10);
   //we read ACK bit here.
   uint8_t ack = sda_read();
   
   //master sets scl low now
   scl_low();
   delayMicroseconds(10/2);
   sda_low(); //master makes sda low too after reading ACK/NAK bit

   return ack;
}

//read one byte. If <last> is true, we send a NAK after having received 
// the byte in order to terminate the read sequence. 
uint8_t i2c_readByte(uint8_t last)
{
  uint8_t b = 0;
  
  sda_high();
  for (uint8_t i = 0; i < 8; ++i)
  {
    b <<= 1;
    delayMicroseconds(10);
    scl_high();
    if (sda_read()) b |= 1;
    scl_low();
  }
  //send NAK
  if (last) sda_high();
  //send ACK
  else sda_low();

  scl_high();
  delayMicroseconds(10/2);
  scl_low();
  delayMicroseconds(10/2);
  sda_low();

  return b;
}

