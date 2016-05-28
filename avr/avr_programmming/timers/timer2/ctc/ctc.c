#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER2_COMP_vect)
{
   PORTB ^= (1 << PB2);
}

/*

   WGM21      WGM20
   1              0   -> CTC mode compare mode based on software
 */
int main()
{
   DDRB |= (1 << PB2); //set PB2 as output

   //init timer2 in ctc mode.
   TCCR2 |= (1 << WGM21) |  //enable CTC mode
      (1 << CS20) | (1 << CS22) | (1 << CS21); // Prescalar = 1024
   TCNT2 = 0;

   OCR2 = 97; // close to 100 ms

   TIMSK |= (1 << OCIE2); //enable compare interrupt for Timer2, this calls TIMER2_COMP_vect
   sei(); //enable global interrupt

   while (1)
     {
        //do nothing. Only overload is ISR for compare which runs in CPU, can we do it better?
     }

   return 0;
}
