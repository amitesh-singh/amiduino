#include <avr/io.h>

static void initTimer0()
{
   TCCR0 |= (1 << CS00);

   //start the timer
   TCNT0 = 0;
}

int main()
{

   DDRC |= (1 << PC0);
   initTimer0();

   while (1)
     {
        //  We use >= 191 instead of ==, as there is a possiblity for the loop code
        // miss it. so to be on safer side, use >=.
        if (TCNT0 >= 191)
          {
             //Do something silly ;)

             TCNT0 = 0;
          }
     }

   return 0;
}
