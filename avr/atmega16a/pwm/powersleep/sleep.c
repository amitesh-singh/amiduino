// CPU speed 1MHz @ Atmega16A,
// timervalue = 250 * 10^-3 * 10^6/1024
// gives 262 ms timeout for each timer0 overflow
// This code shows how to go into deep sleep for 262 ms and wake up after 
// every 262ms and do the job

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/delay.h>

volatile uint8_t f_timer = 0;

ISR(TIMER0_OVF_vect)
{
   if (f_timer == 0)
     {
        f_timer = 1;
     }
}

int main()
{
   DDRB |= (1 << PB3);
   TCCR0 |= (1 << CS00) | (1 << CS02); //prescalar = 1024
   TCNT0 = 0;

   TIMSK |= (1 << TOIE0); //enable timer0 overflow interrupt
   PORTB |= (1 << PB3);
   _delay_ms(1000);
   PORTB &= ~(1 << PB3);
   _delay_ms(1000);
   sei();

   while (1)
     {
        if (f_timer == 1)
          {
             PORTB ^= (1 << PB3);
             f_timer = 0;

             //sleep mode routine
             //sleep bro!
             // only IDLE and  SLEEP_MODE_ADC  seems to work
             set_sleep_mode(SLEEP_MODE_IDLE);

             sleep_enable();
             power_all_disable();

             //now enter into sleep mode
             sleep_mode();

             // you will wake up in every 262 ms ;)
             //wake up
             sleep_disable();
             power_all_enable();
          }
     }

   return 0;
}
