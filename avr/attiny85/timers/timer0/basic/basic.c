#include <avr/io.h>

//For Attiny85 running at 16.5 MHz


static void
_init_timer0()
{
   //TCCR0A = 0; Setting WGM00 & WGM01 = 0 - normal mode
   TCCR0B = (1 << CS00) | ( 1 << CS02); // prescaling = 1024
   TCNT0 = 0; // i need 15 ms delay? 
   //  
   //  = Required Delay / clock time period - 1
   // = 15 * 10^3 / (1 / 8/1024 * 10^6) - 1 = 241
   //
}

void main()
{
   DDRB |= (1 << PB0);

   _init_timer0();

   while (1)
     {
        if (TCNT0 >= 249)
          {
             PORTB ^= (1 << PB0); // toggle LED on every 16 ms
             TCNT0 = 0;
          }
     }
}
