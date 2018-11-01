#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>

static volatile uint16_t sampleCount = 0;
volatile static uint16_t sampleSum = 0;
const static uint8_t maxSampleCount = 32;

ISR(ADC_vect)
{
  if (sampleCount < maxSampleCount)
  {
    sampleSum += ADC;
    ++sampleCount;
    ADCSRA |= (1 << ADSC);
  }
}

void adc_setup()
{
  //REFS0 -if set 1, then use internal reference voltage - 1.1v, then max is 1.1v
  // put some kind of resistor ckt to set the Vmax to 1.1v 
  // if set 0, Vcc is used as reference
   ADMUX |= (1 << MUX0); //this enables ADC1/PB2
   // we need whole 10 bits
   //ADMUX |= (1 << ADLAR); //left justified result.
   ADCSRA |= (1 << ADEN) | (1 << ADIE); //this enables ADC and ADC interrupt
   // the ideal ADC clock is from 50khz to 200Khz for full 10bit resolution
   // faster the clock, faster the conversation takes place.
   //we want division factor to be 8
   // 9.6Mhz/8 = 75Khz -> ADC clock
   ADCSRA |= (1 << ADPS0) | (ADPS1);
}

void processADC()
{
  if (sampleCount < maxSampleCount)
    return;

  uint16_t Vbat; //battery voltage

  Vbat = sampleSum/(sampleCount);
  sampleSum = sampleCount = 0;

  //char buf[5];
  //sprintf(buf, "%d", Vbat);
  //uart_puts(buf);

  ADCSRA |= (1 << ADSC);
}

int main(void)
{
   //uart_puts("ADC code example\r\n");

   adc_setup();
   sei();
   
   ADCSRA |= (1 << ADSC);

   while (1)
     {
       processADC();
       _delay_ms(100);
     }

   return 0;
}
