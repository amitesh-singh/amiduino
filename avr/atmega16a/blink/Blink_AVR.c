#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
   //init
   //DDRB |= 0b0000001;
   DDRB |= (1 << PB0);

   //event loop

   while (1)
     {
        //PORTB = 0b00000001;          /* Turn on first LED bit/pin in PORTB */
        PORTB |= (1 << PB0);          /* Turn on first LED bit/pin in PORTB */
        _delay_ms(1000);                                           /* wait */

        //PORTB = 0b00000000;          /* Turn off all B pins, including LED */
        PORTB = 0x00;          /* Turn off all B pins, including LED */
        _delay_ms(1000);  
     }

   return 0;
}
