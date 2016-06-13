#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>


//
//  PD2 --> IR proximity sensor output, since i have used
// UA741cn IC, LOW = 1.5V, HIGH = 4.1V. It was a bad idea to use 741 op-amp :/
// PB3 - connect Buzzer or speaker 
// Current timer code oscillates at 440Hz.
// f = (f_clock /(2 * N * (1 + OCR0))
// where N - prescalar, 
// Values: f_clock = 1Mhz
//         N = 8
//         f = 440

volatile int8_t isBuzzerPlaying = 0;

//connect speakers at PB3
void playBuzzer()
{
   DDRB |= (1 << PB3); //set PB3 - OC0 at output
   TCCR0 |= (1 << WGM01); // this enables CTC mode
   TCCR0 |= (1 << COM00);   //Toggle OC0 on compare match
   TCCR0 |= (1 << CS01); // Prescalar = 8
   TCNT0 = 0;
   OCR0 = 141; // generate 440Hz sound
}

void stopBuzzer()
{
   TCCR0 = 0x00;

   DDRB &= ~(1 << PB3);
}

ISR(INT0_vect)
{
   isBuzzerPlaying = 1;
}

int main()
{
   GICR |= (1 << INT0);
   MCUCR |= (1 << ISC00);
   sei();

   while (1)
     {
        // when PB2 becomes LOW - when some object is nearby
        if (isBuzzerPlaying == 1)
          {
             playBuzzer();
             _delay_ms(30); //play for 30 ms only
             // now whenever the object is nearby, it will keep playing the sound.
             // if PD2 is still low, keep playing the buzzer
             if ((bit_is_set(PIND, PD2)))
               isBuzzerPlaying = 0;
          }
        else
          {
             stopBuzzer();
             //sleep bro
             // PWR_DOWN/SAVE/ADC/STANDBY did not work, don't know why
             // IDLE seems to work
             set_sleep_mode(SLEEP_MODE_IDLE);

             sleep_enable();
             power_all_disable();
             sleep_mode();

             //wakeup from here
             sleep_disable();
             power_all_enable();
          }
     }

   return 0;
}
