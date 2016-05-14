//#define __AVR_ATmega16A__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
PCINT0 -> PCINT0_vect -> PB0...PB7
PCINT1 -> PCINT1_vect -> PC0..PC7
PCINT2 -> PCINT2_vect -> PD0..PD7

*/
void initPinChangeInt0()
{
#if defined(__AVR_ATmega168P__)
   PCICR |= _BV(PCIE2); // Set pin-change interrupt for D pins
   //PCMSK2 |= _BV(PCINT18); //this should also work.
   PCMSK2 |= _BV(PD2); // PCINT2 at PD2
#elif defined(__AVR_ATmega16A__)
#error ("Atmega16A does not have Pin Change Interrupts (PCINTs) ");
#endif
   sei();
}

int main()
{

  initPinChangeInt0();

   // Event Loop
   while (1)
     {
     }


   return 0;
}
