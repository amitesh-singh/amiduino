//#define __AVR_ATmega16A__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void initPinChangeInt0()
{
#if defined(__AVR_ATmega168P__)
   //PCMSK2 |= _BV(PCINT18); //this should also work.
   PCMSK2 |= _BV(PD2);
   PCICR |= _BV(PCIE2); // Set pin-change interrupt for D pins
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
