//#define __AVR_ATmega16A__ ---> we are passing this macro as compile option in avr-gcc -mmcu=atmega16a
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// LED  - PB2
// Switch - PD2 - on Atmega16, PD2 is INT0
// INT1 - PD3
// INT2 - PB2

// Add this code to check if this code compiles for __AVR_ATmega16A__ only.
#if defined(__AVR_ATmega16A__)
#warning ("This code is compiled for __AVR_ATmega16A__");
#endif

#if defined(__AVR_ATmega168__)
#warning ("This code is compiled for atmega168");
#endif

#if defined(__AVR_ATmega168P__)
#warning ("This code is compiled for atmega168P");
#endif
volatile int8_t button_was_pressed = 0;
void initInterrupt0()
{
   // Interrupt registers are different across chips.
   // The below code works for atmega168 and arduino uno probably
   // change the makefile and change the MCU macro to atmega168p
//   EIMSK |= (1 << INT0); // enable INT0 interrupt
//   EICRA |= (1 << ISC00); // trigger on switch change

   //We are here trying to write code for Atmega16a
   // look into iom16a.h file in /usr/lib/avr/include/avr/
#if defined(__AVR_ATmega16A__)
         7    6
   // [INT1][INT0][INT2][]...   ==> GICR
   GICR |= _BV(INT0); //enable INT0 interrupt
   MCUCR |= _BV(ISC00); // trigger on change (high to low or low to high)
#elif defined(__AVR_ATmega168P__)
   EIMSK |= _BV(INT0); //enable INT0 interrupt, PD0
   EICRA |= _BV(ISC00);
#endif
   sei(); //Set global enable interrupts == asm("SEI");
}

ISR(INT0_vect)
{
   button_was_pressed = 1;
}

int main()
{
   DDRB |= _BV(PB2);
   PORTD |= _BV(PD2); // pullup

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
