#include <avr/io.h>
#include <util/delay.h>
//This program reads PIR sensor digital output at PB0
/*
   PIR IR SENSOR
    _________________
   (_________________)
      |     |     |
     GND  OUTPUT +5V DC
OUTPUT is connected at PB0 digital port of atmega16A
 
 */


int main()
{
   DDRB |= (1 << PB1); // Set PB1 at LED output
   while (1)
     {
        if (bit_is_set(PINB, PB0))
          {
             //The input at PB0 is high
             PORTB |= (1 << PB1);
             _delay_ms(1000);
             PORTB &= ~(1 << PB1);
             _delay_ms(1000);
          }
        else
          {
             //the input at PB0 is low
          }
     }

   return 0;
}
