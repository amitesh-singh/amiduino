#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

//based on https://www.adnbr.co.uk/articles/adc-and-pwm-basics
//
void adc_setup()
{
   ADMUX |= (1 << MUX0); //this enables ADC1/PB2
   ADMUX |= (1 << ADLAR); //left justified result.
   ADCSRA |= (1 << ADEN); //this enables ADC
   // the ideal ADC clock is from 50khz to 200Khz for full 10bit resolution
   // faster the clock, faster the conversation takes place.
   //we want division factor to be 8
   // 9.6Mhz/8 = 75Khz -> ADC clock
   ADCSRA |= (1 << ADPS0) | (ADPS1);
}

int adc_read()
{
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
   // in case of 10 bit, do not set ADLAR and read ADC
   return ADCH;
}

int main(void)
{
   uart_puts("ADC code example\r\n");
   volatile uint8_t adc_in;

   adc_setup();

   while (1)
     {
        adc_in = adc_read();
        _delay_ms(1000);
     }

   return 0;
}
