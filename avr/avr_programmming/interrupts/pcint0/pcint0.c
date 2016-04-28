#define __AVR_ATmega16A__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void initPinChangeInt0()
{
   //PCMSK2 |= _BV(PCINT18); -- this gives compilation error, although it should work
   PCMSK2 |= _BV(PD2);
   PCICR |= _BV(PCIE2); // Set pin-change interrupt for D pins
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
