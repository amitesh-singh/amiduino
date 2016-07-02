#include <avr/io.h>
#include <avr/interrupt.h>

//This is still using CPU, can we use hardware ctc mode instead?
/*
ISR(TIMER0_COMP_vect)
{
   PORTB ^= (1 << PB2);
}
*/

int main()
{
   DDRB |= (1 << PB3); //set PB3 - OC0 at output

   //init timer0 in ctc mode. not sure if its gonna work
   TCCR0 |= (1 << COM00) |  //enable CTC mode
      (1 << CS00) | (1 << CS02); // Prescalar = 1024
   TCNT0 = 0;

   OCR0 = 97; // close to 100 ms

   //we don't need it
   //TIMSK |= (1 << OCIE0); //enable compare interrupt for Timer0
   //sei(); //enable global interrupt

   while (1)
     {
        //do nothing.
     }

   return 0;
}
