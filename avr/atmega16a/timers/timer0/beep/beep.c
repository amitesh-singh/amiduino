#include <avr/io.h>
#include <avr/interrupt.h>


//connect buzzer at PB3 port, it is oscillating 
// at 440Hz.
// formula:
// f = (f_clock/(2 * N * (1 + OCR0)))
// where N - prescalar
int main()
{
   DDRB |= (1 << PB3); //set PB3 - OC0 at output

   //init timer0 in ctc mode. not sure if its gonna work
   TCCR0 |= (1 << WGM01); // this enables CTC mode
   TCCR0 |= (1 << COM00) |  //Toggle OC0 on compare match
      (1 << CS01); // Prescalar = 8
   TCNT0 = 0;

   OCR0 = 141; // close to 100 ms

   while (1)
     {
        //do nothing.
     }

   return 0;
}
