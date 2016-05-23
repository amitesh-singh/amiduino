#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_COMP_vect)
{
   PORTB ^= (1 << PB2);
}

/*

   WGM01      WGM00
   1              0   -> CTC mode compare mode based on software
 */
int main()
{
   DDRB |= (1 << PB2); //set PB2 as output

   //init timer0 in ctc mode. not sure if its gonna work
   TCCR0 |= (1 << WGM01) |  //enable CTC mode
      (1 << CS00) | (1 << CS02); // Prescalar = 1024
   TCNT0 = 0;

   OCR0 = 97; // close to 100 ms

   TIMSK |= (1 << OCIE0); //enable compare interrupt for Timer0
   sei(); //enable global interrupt

   while (1)
     {
        //do nothing. Only overload is ISR for compare which runs in CPU, can we do it better?
     }

   return 0;
}
