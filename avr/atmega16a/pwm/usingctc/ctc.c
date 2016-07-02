//#define F_CPU 1000000UL
// CPU speed 1MHz @ Atmega16A, Trying to get 50% duty cycle 250ms timer?
// timer counter = (250 X 10^-3)/(1024/(1 X 10^6)) - 1 == 242
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(TIMER0_COMP_vect)
{
   PORTB ^= (1 << PB3);
   OCR0 = 242;
}

static void initTimer0()
{
   TCCR0 |= (1 << WGM01) | //enable CTC mode
      (1 << CS00) | (1 << CS02); // prescalar = 1024

   TCNT0 = 0;
   OCR0 = 242;
   TIMSK |= (1 << OCIE0);
   sei();
}

int main()
{

   DDRB |= (1 << PB3);
   //Do a signature blink before actually starting PWM for 250ms on and 250ms off
   PORTB |= (1 << PB3);
   _delay_ms(100);
   PORTB &= ~(1 << PB3);
   _delay_ms(100);
   PORTB |= (1 << PB3);
   _delay_ms(50);
   PORTB &= ~(1 << PB3);
   _delay_ms(50);
   PORTB |= (1 << PB3);
   _delay_ms(50);
   PORTB &= ~(1 << PB3);
   _delay_ms(50);
   initTimer0();

   while (1)
     {
        //do nothing
     }

   return 0;
}
