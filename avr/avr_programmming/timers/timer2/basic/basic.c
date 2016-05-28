#include <avr/io.h>

static void initTimer0()
{
   TCCR2 |= (1 << CS20); //no prescaling

   //start the timer
   TCNT2 = 0;
}

int main()
{

   DDRC |= (1 << PC0);
   initTimer0();

   while (1)
     {
        //  We use >= 191 instead of ==, as there is a possiblity for the loop code
        // miss it. so to be on safer side, use >=.
        if (TCNT2 >= 191)
          {
             //Do something silly ;)
             PORTC ^= (1 << PC0); //toggle PC0

             TCNT2 = 0;
          }
     }

   return 0;
}
