#define __AVR_ATmega16U4__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// LED1  - PB2, LEDSwitch - PB0
// Switch - PD2 - on Atmega16, PD2 is INT0
// INT1 - PD3
// INT2 - PB2
// LED1 is keep toggling and when interrupt is pressed, LEDSwitch is set toggle

void initInterrupt0()
{
   EIMSK |= (1 << INT0); // enable INT0 interrupt, PD2 on Atmega16
   EICRA |= (1 << ISC00); // trigger on switch change
   sei(); //Set global enable interrupts
}

ISR(INT0_vect)
{
   if (bit_is_clear(PIND, PD2))
     {
        // handle debouncing via software.
        // you could just put a Capacitor in between GND and AVR pin 
        _delay_ms(1000);
        if (bit_is_clear(PIND, PD2))
          PORTB ^= _BV(PB0); // toggle led at PB0
     }
}

int main()
{
   DDRB |= _BV(PB2) | _BV(PB0);
   PORTD |= _BV(PD2); // pullup

   initInterrupt0();

   while (1)
     {
        PORTB ^= _BV(PB2);
        _delay_ms(200);
     }

   return 0;
}
