// CPU speed 1MHz @ Atmega16A,
// 45% duty cycle = 45% of 255 = 114.75 = 115
// T = 1/((1/1024) x 10^6 ) = 1.024 x 10^-3 = 1.024 ms = clock cycle
// 115 * 1.024 = 117ms
#include <avr/io.h>

static void pwm_init()
{
   TCCR0 |= (1 << WGM00) | (1 << WGM01) //enable FAST PWM mode
      | (1 << CS00) | (1 << CS02) // prescalar = 1024
      | (1 << COM01); // Clear OC0 on compare match, set OC0 at BOTTOM (non-inverting mode.

   DDRB |= (1 << PB3); // set OC0 port as OUTPUT
}

int main()
{
   pwm_init();

   while (1)
     {
        OCR0 = 115;
     }

   return 0;
}
