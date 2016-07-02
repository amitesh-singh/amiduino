#include <avr/io.h>
#include <util/delay.h>

uint8_t isBuzzerPlaying = 0;

//connect speakers at PB3
void playBuzzer()
{
   DDRB |= (1 << PB3); //set PB3 - OC0 at output
   TCCR0 |= (1 << WGM01); // this enables CTC mode
   TCCR0 |= (1 << COM00);   //Toggle OC0 on compare match
   TCCR0 |= (1 << CS01); // Prescalar = 8
   TCNT0 = 0;
   OCR0 = 141; // close to 100 ms
   isBuzzerPlaying = 1;
}

void stopBuzzer()
{
   TCCR0 = 0x00;

   DDRB &= ~(1 << PB3);
}

int main()
{

   while (1)
     {
        // when PB2 becomes LOW - when some object is nearby
        if (!(bit_is_set(PINB, PB0)) && isBuzzerPlaying == 0)
          {
             playBuzzer();
          }
        else if (bit_is_set(PINB, PB0) && isBuzzerPlaying == 1)
          {
             stopBuzzer();
             isBuzzerPlaying = 0;
          }
     }

   return 0;
}
