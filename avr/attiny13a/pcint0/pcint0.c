#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//
// PB1 - PCINT1
// PB2 - LED connected
ISR(PCINT0_vect)
{
   PORTB ^= (1 << PB2);
}

int main(void)
{
   DDRB |= (1 << PB2); // LED

   DDRB &= ~(1 << PB1); // Switch!! PCINT1
   PORTB |= (1 << PB1); // pullup

   PCMSK |= (1 << PCINT0); // ISR will get called on any logical change.
                           // or PCINT0..5. check DS for pin diagram
   GIMSK |= (1 << PCIE); // enable PCINTs 

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
