#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

//CPU is running at 4.8MHZ and baud rate is 38400

int main(void)
{
   DDRB |= (1 << PB0);

   uart_puts("Hello\r\n");
   //event loop

   while (1)
     {
        PORTB ^= _BV(PB0);
        _delay_ms(1000);
               
        uart_puts("Led blinks\r\n");
     }

   return 0;
}
