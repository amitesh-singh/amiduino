#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN PB0

ISR(TIM0_COMPA_vect)
{
   PORTB ^= _BV(LED_PIN);
}

int main()
{
   DDRB |= _BV(LED_PIN);

   cli();

   //timer init code.
   TCCR0A |= _BV(WGM01); // set timer counter mode to CTC
   TCCR0B |= _BV(CS00) | _BV(CS02); //f_cpu/1024
   OCR0A = 255; //set at max value.

   TIMSK0 |= _BV(OCIE0A); //enable timer CTC interrupt.

   sei();


   while (1)
     {
     }

   return 0;
}
