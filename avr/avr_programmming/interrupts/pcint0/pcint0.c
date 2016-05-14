#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
PCINT0 -> PCINT0_vect -> PB0...PB7
PCINT1 -> PCINT1_vect -> PC0..PC7
PCINT2 -> PCINT2_vect -> PD0..PD7

*/

volatile uint8_t button_pressed = 0;
void initPinChangeInt0()
{
#if defined(__AVR_ATmega168P__)
   PCICR |= _BV(PCIE0); // Set pin-change interrupt for B pins
   PCMSK2 |= _BV(PB2); // PCINT0 at PB2
#elif defined(__AVR_ATmega16A__)
#error ("Atmega16A does not have Pin Change Interrupts (PCINTs) ");
#endif
   sei();
}

ISR(PCINT0_vect)
{
   //INT0 interrupt function
   if (bit_is_clear(PINB, PB2))
       {
          button_pressed = 1;
       }
}

int main()
{
   DDRD |= _BV(PD2); // Set PD2 as OUTPUT
   PORTB |= _BV(PB2); // enable pull up at PB2
   initPinChangeInt0();

   // Event Loop
   while (1)
     {
        if (button_pressed == 1)
          {
             // Turn on LED at PD2
             PORTD = _BV(PD2);
             _delay_ms(4000);
             button_pressed = 0;
             PORTD = 0x0; // turn off the led at PD2
          }
     }

   return 0;
}
