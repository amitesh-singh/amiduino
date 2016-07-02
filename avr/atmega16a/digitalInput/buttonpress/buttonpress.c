#include <avr/io.h>
#include <util/delay.h>

//
// LED is connected at PB2
// Switch is connected at PB0
//
//

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
        if (bit_is_clear(PINB, PB0))
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
