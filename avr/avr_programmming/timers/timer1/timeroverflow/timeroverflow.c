#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t tot_overflow;

static void initTimer0()
{
   TCCR1B |= ((1 << CS10) | (1 << CS12)); // prescalar = 1024

   //start the timer
   TCNT1 = 0;
}

ISR(TIMER1_OVF_vect)
{
   ++tot_overflow;
}

int main()
{
   DDRD |= (1 << PD7); //set PD7 output
   initTimer0();

   //enable overflow interrutp
   TIMSK |= (1 << TOIE1);
   //enable global interrupt
   sei();

   tot_overflow = 0;

   while (1)
     {
        if (tot_overflow >= 3)
          {
             if (TCNT2 >= 231)
               {
                  //this should toggle the LED at every 201008.128 ms = 201 seconds which is quite large :)
                  PORTD ^= (1 << PD7); // toggle at PD7
                  TCNT1 = 0; //reset the timer to zero.
                  tot_overflow = 0; //reset the overflow counter too
               }
          }
     }

   return 0;
}
