//using one 8 bit pwm to control two outputs
#include <avr/io.h>

int main(void)
{
    // PD6 is now an output
    DDRD |= (1 << PD6);
    // PD5 is now 2nd output
    DDRD |= (1 << PD5);

    OCR0A = 128; //50% duty cycle
    OCR0B = 250; // ~ 100 % duty cycle

    // set none-inverting mode at both pins
    TCCR0A |= (1 << COM0A1); //enable OC0A pin
    TCCR0A |= (1 << COM0B1); //enable OC0B pin

    // set fast PWM Mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00);

    TCCR0B |= (1 << CS01);
    // set prescaler to 8 and starts PWM

    while (1);
    {
        // we have a working Fast PWM
    }
}
