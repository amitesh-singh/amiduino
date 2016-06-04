#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
   //init
   //DDRB |= 0b0000001;
   DDRB |= (1 << PB0);
   DDRB |= (1 << PB1);
   DDRB |= (1 << PB2);
   DDRB |= (1 << PB3);
   DDRB |= (1 << PB4);
   DDRB |= (1 << PB5);

   //event loop

   while (1)
     {
        //PORTB = 0b00000001;          /* Turn on first LED bit/pin in PORTB */
        PORTB |= (1 << PB0);          /* Turn on first LED bit/pin in PORTB */
        PORTB |= (1 << PB1);          /* Turn on first LED bit/pin in PORTB */
        PORTB |= (1 << PB2);          /* Turn on first LED bit/pin in PORTB */
        PORTB |= (1 << PB3);          /* Turn on first LED bit/pin in PORTB */
        PORTB |= (1 << PB4);          /* Turn on first LED bit/pin in PORTB */
        PORTB |= (1 << PB5);          /* Turn on first LED bit/pin in PORTB */
        _delay_ms(1000);                                           /* wait */

        //PORTB = 0b00000000;          /* Turn off all B pins, including LED */
        PORTB = 0x00;          /* Turn off all B pins, including LED */
        _delay_ms(1000);  
     }

   return 0;
}
