//using one 16 bit pwm to control one output - OC1A
//PB1 - OC1A
// PB2 - OC2A
// Fast pwm mode: pwm_f = clock_speed / [Prescalar] (1 + Top)
#include <avr/io.h>

int main(void)
{
    // PB1 is now an output
    DDRB |= (1 << PB1);

    OCR1A = 0x3FFF; //25% duty cycle
    // set none-inverting mode at both pins
    TCCR1A |= (1 << COM1A1); //enable OC1A pin

    // set FAST pwm using  ICR1 as TOP
    TCCR1A |= (1 << WGM11);
    TCCR1A |= (1 << WGM12) | (1 << WGM13); 

    TCCR1B |= (1 << CS11);
    // set prescaler to 8 and starts PWM

    while (1);
    {
        // we have a working Fast PWM
    }
}
