//#define __AVR_ATmega16A__ ---> we are passing this macro as compile option in avr-gcc -mmcu=atmega16a
#include <avr/io.h>
#include <util/delay.h>

//This program checks all digital ports
// and do a classic LED blink on all ports
// PORTAs are the analog ones, read only
int main()
{
   DDRB = 0xff;
   DDRC = 0xff;
   DDRD = 0xff;

   while (1)
     {
        PORTB = 0xff;
        PORTC = 0xff;
        PORTD = 0xff;
        _delay_ms(100);
        PORTB = 0;
        PORTC = 0;
        PORTD = 0;
        _delay_ms(100);

        PORTB = 0xff;
        PORTC = 0xff;
        PORTD = 0xff;
        _delay_ms(60);
        PORTB = 0;
        PORTC = 0;
        PORTD = 0;
        _delay_ms(60);
     }

   return 0;
}
