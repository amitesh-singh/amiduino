#include <avr/io.h>

static void initTimer1()
{
   TCCR1B |= (1 << CS10); // prescaling = 1

   //start the timer
   TCNT1 = 0;
}

int main()
{

   DDRD |= (1 << PD7);
   initTimer1();

   while (1)
     {
        //  We use >= 65345 instead of ==, as there is a possiblity for the loop code
        // miss it. so to be on safer side, use >=.
        if (TCNT1 >= 65345)
          {
             //Do something silly ;)
             PORTD ^= (1 << PD7); //toggle PD7, 65.346 ms toggle
             // do the math

             TCNT1 = 0;
          }
     }

   return 0;
}
