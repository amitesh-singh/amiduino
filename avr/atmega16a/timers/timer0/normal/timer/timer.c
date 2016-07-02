#include <avr/io.h>

static void initTimer0()
{
   TCCR0 |= ((1 << CS00) | (1 << CS02));

   //start the timer
   TCNT0 = 0;
}

int main()
{
   DDRB |= (1 << PB2); //set PB2 output
   initTimer0();

   while (1)
     {
        //  We use >= 99 instead of ==, as there is a possiblity for the loop code
        // miss it. so to be on safer side, use >=.
        if (TCNT0 >= 99)
          {
             //this should toggle the LED at every 102.4 ms
             PORTB ^= (1 << PB2); // toggle at PB2
             TCNT0 = 0; //reset the timer to zero.
          }
     }

   return 0;
}
