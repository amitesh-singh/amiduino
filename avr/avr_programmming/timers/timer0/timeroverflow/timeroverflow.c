#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t tot_overflow;

static void initTimer0()
{
   TCCR0 |= ((1 << CS00) | (1 << CS02));

   //start the timer
   TCNT0 = 0;
}

ISR(TIMER0_OVF_vect)
{
   ++tot_overflow;
}

int main()
{
   DDRB |= (1 << PB2); //set PB2 output
   initTimer0();

   //enable overflow interrutp
   TIMSK |= (1 << TOIE0); 
   //enable global interrupt
   sei();

   tot_overflow = 0;

   while (1)
     {
        if (tot_overflow >= 3)
          {
             if (TCNT0 >= 231)
               {
                  //this should toggle the LED at every 1024 ms
                  PORTB ^= (1 << PB2); // toggle at PB2
                  TCNT0 = 0; //reset the timer to zero.
                  tot_overflow = 0; //reset the overflow counter too
               }
          }
     }

   return 0;
}
