#include <avr/io.h>
#include <util/delay.h>

int main()
{
   DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
   PORTB = 0x00;

   while (1)
     {
        //1000 1100 0100 0110 0010 0011 0001 1001 - half step - clockwise

        PORTB = (1 << PB0);
        _delay_ms(10);

        PORTB = (1 << PB0) | (1 << PB1);
        _delay_ms(10);

        PORTB = (1 << PB1);
        _delay_ms(10);

        PORTB = (1 << PB1) | (1 << PB2);
        _delay_ms(10);

        PORTB = (1 << PB2);
        _delay_ms(10);

        PORTB = (1 << PB2) | (1 << PB3);
        _delay_ms(10);

        PORTB = (1 << PB3);
        _delay_ms(10);

        PORTB = (1 << PB0) | (1 << PB3);
        _delay_ms(10);
     }

   return 0;
}
