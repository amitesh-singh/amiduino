// timer2 - pwm2 example - one output OCR2A pin
// PB3 - OCR2A pin
// PD3 - OCR2B pin

#include <avr/io.h>


int main()
{
    DDRB |= (1 << PB3);

    OCR2A = 200; //50% duty cycle

    TCCR2A |= (1 << COM2A1); //enable OCR2A pin with non-inverted mode

    TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << WGM22); // Fast pwm - ORC2A top?

    TCCR2B |= (1 << CS21); // set prescaler to 8 and start pwm
    while (1)
    {

    }

    return 0;
}