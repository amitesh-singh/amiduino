#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

//based on https://www.adnbr.co.uk/articles/adc-and-pwm-basics
int main(void)
{
   uart_puts("ADC code example\r\n");
   uint8_t adc_in;

   //setup ADC

   ADMUX |= (1 << MUX0); //this enables ADC1/PB2
   ADMUX |= (1 << ADLAR); //left justified result.
   ADCSRA |= (1 << ADEN); //this enables ADC
   //we want division factor to be 8
   // 9.6Mhz/8 = 75Khz -> ADC clock
   ADCSRA |= (1 << ADPS0) | (ADPS1);

   //start the conversion
   ADCSRA |= (1 << ADSC);

   //Each conversion takes 13 ADC clock cycles (bar the first one, which takes 25.) 
   //wait until ADSC becomes 0
   while (ADCSRA & (1 << ADSC))
     {
        __asm__ volatile ("nop");
     }

   //since we adjusted the  result to be left adjusted 
   // and since PWM does not support more than 8 bit so why 
   // should we read ADC (10 bit resolution). Lets read only ADCH
   adc_in = ADCH;

   // Setup PWM
   //This AVR microcontroller has two channels with 8-bit PWM capability
   // - PB0/OCOA (pin 5) and PB1/OCOB (pin 6), both running from Timer 0.
   // Set Timer 0 prescaler to clock/8.
   // At 9.6 MHz this is 1.2 MHz.
   TCCR0B |= (1 << CS01) | (1 << CS00);

   //formula is 
   // f(pwm) = f(sysclock)/(P * M);
   // P - prescaler factor
   // M - depens upon PWM mode
   // if fast mode - 256
   // if phase correct pwm - 512
   // for M = 256, fast pwm mode
   // give a frequency of 4.7 kHz - more than fast enough to avoid eye-strain-inducing flicker.
   // Set to 'Fast PWM' mode
   TCCR0A |= (1 << WGM01) | (1 << WGM00);

   // Clear OC0B output on compare match, upwards counting.
   TCCR0A |= (1 << COM0B1);

   OCR0B = adc_in;
   while (1)
     {
        //READ ADC START
        //start the conversion
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC))
          {
             __asm__ volatile ("nop");
          }
        adc_in = ADCH;
        // READ ADC END..

        OCR0B = adc_in;
     }

   return 0;
}
