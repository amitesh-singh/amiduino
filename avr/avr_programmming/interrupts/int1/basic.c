//#define __AVR_ATmega16U4__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// LED  - PB2
// Switch - PD3 - on Atmega16, PD3 is INT1


#if defined(__AVR_ATmega16A__)
#warning ("This code is getting compiled for AVR Atmega16A chip");
#endif

#if defined(__AVR_ATmega168P__)
#warning ("This code is getting compiled for Atmega168p");
#endif

//Always declare the global variable volatile if its shared between main and ISRs.
// This is to tell compiler not to optimize the codeblock related to this variable.
volatile int8_t button_was_pressed = 0;

void initInterrupt0()
{
   // or arduino uno may be? Atmega328p
#if defined(__AVR_ATmega168P__)
   EIMSK |= (1 << INT1); // enable INT1 interrupt
   EICRA |= (1 << ISC00); // trigger on switch change, either from 0V to 5V or 5V to 0V.
#elif defined(__AVR_ATmega16A__)
   GICR |= _BV(INT1); // enable INT1 interrupt PD3 port of Atmega16a
   MCUCR |= _BV(ISC00);
#endif
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
