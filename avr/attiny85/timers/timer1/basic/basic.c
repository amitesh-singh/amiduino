#include <avr/io.h>
#include <avr/interrupt.h>

//For Attiny85 running at 16.5 MHz
// timer1 is weird in many ways
//1 weirdness is 
// from t85 DS: page no. 85: In the normal mode an overflow interrupt (TOV1) is generated when Timer/Counter1 counts from $FF to $00

static void
_init_timer0()
{
   TCCR1 = (1 << CS13) | ( 1 << CS12); // prescaling = 2048
   TCNT1 = 0xFF;
   //TCNT1 = 256 or -1; (0xFF)
   //  
   //  = Required Delay / clock time period - 1
   // = 22 * 10^3 / (1 / 16.5/2048 * 10^6) - 1 = 176  == 255 - 176
   //
}

void main()
{
   DDRB |= (1 << PB0);

   _init_timer0();

   while (1)
     {
        if (TCNT1 <= (255 - 176))
          {
             PORTB ^= (1 << PB0); // toggle LED on every 16 ms
             TCNT1 = 0xFF;
          }
     }
}
