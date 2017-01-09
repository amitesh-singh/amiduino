#include <avr/io.h>
#include <util/delay.h>

/*
formula:
SCL = F_CPU/ (16 + 2*TWBR * prescalar value);

 */
static void
i2c_init()
{
   TWSR = 0; //prescalar: 1, 4, 16, 64 (TWPS1, TWPS2)
   TWBR = 12; // 16Mhz/(16 + 2 * 12 * 1) = 16Mhz/40 = 400 Khz
   //TWCR - 
   // enable TWI interrupt, TWI enable, Start, Stop
   TWCR = (1 << TWEN); // enable TWI
}

static void
i2c_start()
{       // interrupt,    Start          TWI enable
   TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
   while ((TWCR & (1 << TWINT)) == 0);
}

static void
i2c_stop()
{                        // Stop
   TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

static void
i2c_write(uint8_t data)
{
   TWDR = data;
   TWCR = (1 << TWINT) | (1 << TWEN);
   while ((TWCR & (1 << TWINT)) == 0);
}

//this transfer ack byte after byte transfer
static uint8_t
i2c_readack()
{
   TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
   while ((TWCR & (1 << TWINT)) == 0);
   return TWDR;
}

//This does not send ack 
static uint8_t
i2c_readnack()
{
   TWCR = (1 << TWINT) | (1 << TWEN);
   while ((TWCR && (1 << TWINT)) == 0);
   return TWDR;
}

//this is used to check the error in transmission if any.
static uint8_t
i2c_getstatus()
{
   uint8_t status;

   status = TWSR & 0xF8;

   return status;
}

int main()
{
   i2c_init();
   while (1)
     {
     }

   return 0;
}
