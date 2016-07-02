#include <avr/io.h>

/*
   TCCR2 = [.......][CS22][CS21][CS20]
*/

static void initTimer0()
{
   //enable prescalar = 128
   TCCR2 |= ((1 << CS20) | (1 << CS22));

   //start the timer
   TCNT2 = 0;
}

int main()
{
   DDRB |= (1 << PB2); //set PB2 output
   initTimer0();

   while (1)
     {
        //  We use >= 99 instead of ==, as there is a possiblity for the loop code
        // miss it. so to be on safer side, use >=.
        if (TCNT2 >= 99)
          {
             //this should toggle the LED at every 102.4 ms
             PORTB ^= (1 << PB2); // toggle at PB2
             TCNT2 = 0; //reset the timer to zero.
          }
     }

   return 0;
}
