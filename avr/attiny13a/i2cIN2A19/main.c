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
   ack = i2c_start((64 << 1) + 1);
   i2c_stop();

   if (ack == 0)
     {
        uart_puts("got the slave i2c device.\r\n");
     }
}

int main(void)
{
   uart_puts("Looking for i2c device INA219.\r\n");
   //wait for 200ms 
   _delay_ms(200);
   i2c_scan();

   while (1)
     {
     }

   return 0;
}
