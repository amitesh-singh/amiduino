#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

int main(void)
{

   // Tuning of the internal oscillator is controlled by the oscillator calibration register, OSCCAL.
  // When the microcontroller starts the factory calibration value is automatically loaded into the OSCCAL register.
  // You cannot modify the factory calibration value, but your program can change the OSCCAL register at runtime.
   //Tuning the oscillator by adjusting OSCCAL
   /*
      Using a delta adjustment of the factory calibrated value can be better than baking in the actual OSCCAL value if you don’t want to tune each chip individually.
    */

   uart_putc(OSCCAL);
   //in my testing it was working fine from OSCCAL + 4 to OSCCAL - 6
   OSCCAL -= 1;
   uart_putc(0);
   uart_putc(OSCCAL);
   //uart_puts("clock is calibrated\r\n");

   while (1)
     {
     }

   return 0;
}
