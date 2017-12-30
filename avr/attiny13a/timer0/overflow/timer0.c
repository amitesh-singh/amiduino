#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>
#include <avr/power.h>

#define LED_PORT PB0

//This shows how to to enable timer overflow interrupt
//
static void
init_timer0()
{
    //Note: attiny13a has only one timer i.e. timer0 8 bit.
    cli();

    TCCR0B |= (1 << CS02) | (1 << CS00); // f_cpu/1024

    //enable timer overflow interrupt
    TIMSK0 |= 1 << TOIE0;

    //enable global interrupt
    sei();
}

static void
stop_timer()
{
    cli();
    TIMSK0 &= ~(1 << TOIE0);
    sei();
}

//ISR would be called when TCNT reaches 255.
ISR(TIM0_OVF_vect)
{
    PORTB ^= (1 << LED_PORT);
}

int main(void)
{
    //clock_prescale_set(clock_div_4);

    DDRB |= (1 << LED_PORT);

    init_timer0();

    PORTB |= (1 << LED_PORT);
    _delay_ms(1000);
    PORTB &= ~(1 << LED_PORT);
    _delay_ms(1000);

    while (1) { }

    return 0;
}
