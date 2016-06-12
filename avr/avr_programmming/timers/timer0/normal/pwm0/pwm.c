#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{
   DDRB |= (1 << PB3); //set PB3 - OC0 at output

   //init timer0 in normal mode since WGM00 and WGM01 are 0
   TCCR0 |= (1 << COM00) |  //non pwm node, Toggle OC0 on compare match
      (1 << CS00) | (1 << CS02); // Prescalar = 1024
   TCNT0 = 0;

   // Note: in normal mode, TOP is 0xFF (255) (Refer DS, in CTC mode,
  // TOP is actually OCR0) hence we would get ON and OFF when TIMER0 is actually overflow
   // 1024  * 256 /1000 = 262 ms ON OFF signal
   //OCR0 = 97; // close to 100 ms

   //we don't need it
   //TIMSK |= (1 << OCIE0); //enable compare interrupt for Timer0
   //sei(); //enable global interrupt

   while (1)
     {
        //do nothing.
     }

   return 0;
}
