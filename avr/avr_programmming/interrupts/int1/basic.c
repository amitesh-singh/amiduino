#define __AVR_ATmega16U4__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// LED  - PB2
// Switch - PD3 - on Atmega16, PD3 is INT1

//Always declare the global variable volatile if its shared between main and ISRs.
// This is to tell compiler not to optimize the codeblock related to this variable.
volatile int8_t button_was_pressed = 0;

void initInterrupt0()
{
   EIMSK |= (1 << INT1); // enable INT0 interrupt
   EICRA |= (1 << ISC00); // trigger on switch change, either from 0V to 5V or 5V to 0V.
   sei(); //Set global enable interrupts
}

ISR(INT1_vect)
{
   button_was_pressed = 1;
}

int main()
{
   DDRB |= _BV(PB2);
   PORTD |= _BV(PD3); // pullup

   initInterrupt0();

   while (1)
     {
        if (button_was_pressed == 1)
          {
             PORTB ^= _BV(PB2); // toggle LED
             _delay_ms(1000);
             // TODO: make below statement atomic though?
             // Probably not needed since used int8_t type and MCU is of 8 bits type hence the below statement should be single statement in asm?
             button_was_pressed = 0;
          }
     }

   return 0;
}
