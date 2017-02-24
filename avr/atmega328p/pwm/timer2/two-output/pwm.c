// timer2 - pwm2 example - one output OCR2A pin
// PB3 - OCR2A pin
// PD3 - OCR2B pin

#include <avr/io.h>


int main()
{
    DDRB |= (1 << PB3);
    DDRD |= (1 << PD3);

    OCR2A = 200; //80% duty cycle
    OCR2B = 255; //40% duty cycle

    TCCR2A |= (1 << COM2A1); //enable OCR2A pin with non-inverted mode
    TCCR2A |= (1 << COM2B1); //enable OCR2B pin with non-inverted mode
    TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << WGM22); // Fast pwm - ORC2A top?

    TCCR2B |= (1 << CS21) | (1 << CS20) | (1 << CS22); // set prescaler to 8 and start pwm

    while (1)
    {

    }

    return 0;
}
