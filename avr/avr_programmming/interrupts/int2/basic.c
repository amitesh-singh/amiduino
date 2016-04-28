#define __AVR_ATmega16U4__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// LED  - PB0
// Switch - PB2

volatile int8_t button_was_pressed = 0;
void initInterrupt0()
{
   EIMSK |= (1 << INT2); // enable INT2 interrupt, PB2
   EICRA |= (1 << ISC00); // trigger on switch change
   sei(); //Set global enable interrupts
}

ISR(INT0_vect)
{
   button_was_pressed = 1;
}

int main()
{
   DDRB |= _BV(PB0);
   PORTB |= _BV(PB2); // pullup

   initInterrupt0();

   while (1)
     {
        if (button_was_pressed == 1)
          {
             PORTB ^= _BV(PB0); // toggle LED
             _delay_ms(1000);
             // TODO: make below statement atomic though?
             // Probably not needed since used int8_t type and MCU is of 8 bits type hence the below statement should be single statement in asm?
             button_was_pressed = 0;
          }
     }

   return 0;
}
