#include <util/delay.h>
#include "i2csoft.h"
#include "uart.h"
#include <stdlib.h>

//this is tested on i2c module to calculate current
// i2c address is 64 (0x40 in hex)
// 
void i2c_scan()
{
   i2c_init();
   uint8_t ack = 1;

   for (uint8_t i = 8; i < 128; ++i)
     {
        ack = i2c_start((i << 1) + 1);
        i2c_stop();

        if (ack == 0)
          {
             uart_puts("got the slave i2c device.\r\n");
             //probably most efficient way to convert int to string
             char buf[5];
             itoa(i, buf, 10);
             uart_puts(buf);
          }
        _delay_ms(40);
     }
}

int main(void)
{
   uart_puts("Starting i2c scanner.\r\n");
   //wait for 2 seconds
   _delay_ms(200);
   i2c_scan();

   while (1)
     {
     }

   return 0;
}
