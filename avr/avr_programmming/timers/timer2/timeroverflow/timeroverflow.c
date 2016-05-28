#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t tot_overflow;

static void initTimer0()
{
   // Prescalar = 1024
   TCCR2 |= ((1 << CS20) | (1 << CS22) | (1 << CS21));

   //start the timer
   TCNT2 = 0;
}

ISR(TIMER2_OVF_vect)
{
   ++tot_overflow;
}

int main()
{
   DDRB |= (1 << PB2); //set PB2 output
   initTimer0();

   //enable overflow interrupt for Timer2
   // Timer/counter2 overflow interrupt enable
   TIMSK |= (1 << TOIE2);
   //enable global interrupt
   sei();

   tot_overflow = 0;

   while (1)
     {
        if (tot_overflow >= 3)
          {
             if (TCNT2 >= 231)
               {
                  //this should toggle the LED at every 1024 ms
                  PORTB ^= (1 << PB2); // toggle at PB2
                  TCNT2 = 0; //reset the timer to zero.
                  tot_overflow = 0; //reset the overflow counter too
               }
          }
     }

   return 0;
}
