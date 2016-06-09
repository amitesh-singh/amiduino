// CPU speed 1MHz @ Atmega16A,
// 45% duty cycle = 45% of 255 = 114.75 = 115
// T = 1/((1/1024) x 10^6 ) = 1.024 x 10^-3 = 1.024 ms = clock cycle
// 115 * 1.024 = 117ms
#include <avr/io.h>
#include <util/delay.h>

static void pwm_init()
{
   TCCR0 |= (1 << WGM00) | (1 << WGM01) //enable FAST PWM mode
      | (1 << CS00) // no prescalar
      | (1 << COM01);

   DDRB |= (1 << PB3); // set OC0 port as OUTPUT
}

int main()
{
   uint8_t brightness;
   pwm_init();

   while (1)
     {
        for (brightness = 0; brightness < 255; ++brightness)
          {
             OCR0 = brightness;
             _delay_ms(10);
          }
        for (brightness = 255; brightness > 0; --brightness)
          {
             OCR0 = brightness;
             _delay_ms(10);
          }
     }

   return 0;
}
