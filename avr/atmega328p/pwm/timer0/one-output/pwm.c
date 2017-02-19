//using one 8 bit pwm to control one output - OC0A
#include <avr/io.h>

int main(void)
{
    // PD6 is now an output
    DDRD |= (1 << PD6);

    OCR0A = 128; //50% duty cycle

    // set none-inverting mode at both pins
    TCCR0A |= (1 << COM0A1); //enable OC0A pin

    // set fast PWM Mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00);

    TCCR0B |= (1 << CS01);
    // set prescaler to 8 and starts PWM

    while (1);
    {
        // we have a working Fast PWM
    }
}
