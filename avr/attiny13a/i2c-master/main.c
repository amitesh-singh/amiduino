#include <util/delay.h>
#include "i2csoft.h"
#include "uart.h"

void i2c_scan()
{
   i2c_init();
   uint8_t ack = 1;
   uint8_t gotOneSlaveAtleast = 0;

   for (uint8_t i = 8; i < 128; ++i)
     {
        ack = start((i << 1) + 1);
        stop();

        if (ack == 0)
          {
             uart_puts("got the slave i2c device.\r\n");
             gotOneSlaveAtleast = 1;
          }
        _delay_ms(400);
     }

     if (!gotOneSlaveAtleast)
     {
       uart_puts("did not get slave i2c device :/\r\n");
     }
    
}

int main(void)
{
   uart_puts("Starting i2c scanner.\r\n");
   //wait for 2 seconds
   _delay_ms(2000);

   i2c_scan();

   while (1)
     {
     }

   return 0;
}
