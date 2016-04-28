#include <avr/io.h>
#include <util/delay.h>

//
// LED is connected at PB2
// Switch is connected at PB0
//
//

// there are two ways to solve debouncing problem
// 1. software debouncing - you save few Ruppees on capacitors.
// 2. Hardware debouncing - connect a capacitor between switch 
//        Switch
//        o---o
// GND ----   ------------ AVR (PB0)
//      | -  +  |
//      |--||---|
//         C (0.1 uF)
// if the circuit is like above, you actually don't need debounce().
// Refer buttonpress.c 
int8_t debounce()
{
   if (bit_is_clear(PINB, PB0))
     {
        //there might be some fluctuation, read the port again after 1 s
        _delay_ms(1000);
        if (bit_is_clear(PINB, PB0))
          return 1;
     }

   return 0;
}

int main()
{
   uint8_t button_is_pressed = 0;
   DDRB |= _BV(PB2);// set PB2 for Output, Note: PB0 is set for input
    // 0b00000100

   PORTB |= _BV(PB0); // enable PULLup
   // PB0 is set at high voltage (5V)
   // when button/switch is pressed, the voltage is set LOW (0V).

   // Event Loop
   while (1)
     {
        if (debounce())
          {
             if (button_is_pressed == 0)
               {
                  PORTB ^= _BV(PB2); //toggle the LED state
                  button_is_pressed = 1;
               }
          }
        else
          {
             button_is_pressed = 0;
          }
     }

   return 0;
}
