#include <avr/io.h>
#include <util/delay.h>


static void
stopMotor()
{
   PORTB = 0;
}

int main()
{
   int i;
   DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
   PORTB = 0x00;

   while (1)
     {
        //Stepper Squence 1001,1100,0110,0011 - clockwise

        //180 degree - Clockwise
        // 256 * 4 = 1024 - total steps to complete one rotation = 2048
        for (i = 0; i < 256; ++i)
          {
             PORTB = (1 << PB0) | (1 << PB3);
             _delay_ms(4);

             PORTB = (1 << PB0) | (1 << PB1);
             _delay_ms(4);

             PORTB = (1 << PB1) | (1 << PB2);
             _delay_ms(4);

             PORTB = (1 << PB2) | (1 << PB3);
             _delay_ms(4);
          }
        stopMotor();
        _delay_ms(2000);

        // 180 degree anti-clockwise
        for (i = 0; i < 256; ++i)
          {
             PORTB = (1 << PB2) | (1 << PB3);
             _delay_ms(4);

             PORTB = (1 << PB1) | (1 << PB2);
             _delay_ms(4);

             PORTB = (1 << PB0) | (1 << PB1);
             _delay_ms(4);

             PORTB = (1 << PB0) | (1 << PB3);
             _delay_ms(4);
          }
        stopMotor();
        _delay_ms(2000);
     }

   return 0;
}
