#include <avr/io.h>
#include <util/delay.h>

int main()
{
   DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
   PORTB = 0x00;

   while (1)
     {
        //Stepper Squence 1001,1100,0110,0011 - clockwise

        PORTB = (1 << PB0) | (1 << PB3);
        _delay_ms(4);

        PORTB = (1 << PB0) | (1 << PB1);
        _delay_ms(4);

        PORTB = (1 << PB1) | (1 << PB2);
        _delay_ms(4);

        PORTB = (1 << PB2) | (1 << PB3);
        _delay_ms(4);
     }

   return 0;
}
