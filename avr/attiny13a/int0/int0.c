#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//
// PB1 - INT0
// PB2 - LED connected
ISR(INT0_vect)
{
   PORTB ^= (1 << PB2);
}

int main(void)
{
   DDRB |= (1 << PB2); // LED

   DDRB &= ~(1 << PB1); // Switch!! INT0
   PORTB |= (1 << PB1); // pullup

   GIMSK |= (1 << INT0); // the external interrupt in enabled.
   MCUCR |= (1 << ISC00); // any logical change on int0 generates an interrupt request.
   //ISC01 - falling edge of int0
   // isc00 and isc01: both 1 - the rising edge of INT0
   sei(); //set global enable interrupts!

   _delay_ms(1000);
   PORTB |= (1 << PB2);
   _delay_ms(1000);
   PORTB &= ~(1 << PB2);

   while (1)
     {
     }

   return 0;
}
