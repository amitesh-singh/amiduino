#include <avr/io.h>
#include <avr/interrupt.h>

//This is still using CPU, can we use hardware ctc mode instead?
/*
ISR(TIMER2_COMP_vect)
{
   PORTB ^= (1 << PB2);
}
*/

int main()
{
   DDRD |= (1 << PD7); //set PD7- OC2 at output

   //init timer2 in ctc mode.
   TCCR2 |= (1 << WGM21); // enable CTC mode: Refer DS
   TCCR2 |= (1 << COM20) |  //non-pwm mode - Toggle OC2 on compare match
      (1 << CS20) | (1 << CS22) | (1 << CS21); // Prescalar = 1024
   TCNT2 = 0;

   OCR2 = 97; // close to 100 ms

   //we don't need it
   //TIMSK |= (1 << OCIE2); //enable compare interrupt for Timer0
   //sei(); //enable global interrupt

   while (1)
     {
        //do nothing.
     }

   return 0;
}
