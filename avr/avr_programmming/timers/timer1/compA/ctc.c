#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect)
{
   PORTD ^= (1 << PD7);
}

/*

   WGM01      WGM00
   1              0   -> CTC mode compare mode based on software
 */
int main()
{
   DDRD |= (1 << PD7); //set PB2 as output

   TCCR1B |= (1 << CS10); // no prescalar, normal mode
   TCNT1 = 0;

   OCR1A = 97; // close to 100 ms

   TIMSK |= (1 << OCIE1A); //enable compare interrupt for Timer1 for OCR1A
   sei(); //enable global interrupt

   while (1)
     {
        //do nothing. Only overload is ISR for compare which runs in CPU, can we do it better?
     }

   return 0;
}
