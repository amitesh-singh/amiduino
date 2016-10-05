#include <avr/io.h>
#include <util/delay.h>

#if defined(__AVR_ATmega16A__)
#warning ("This code is compiled for __AVR_ATmega16A__");
#endif

static void
adc_init()
{
   // ADC Multiplexer Selection Register
   ADMUX = (1 << REFS0);

   // ADCSRA - ADC control and status Register A
   // ADEN - ADC enable
   ADCSRA = (1 << ADEN) |
      (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
}

static uint16_t
adc_read(uint8_t ch)
{
   // make sure value remains between 0 and 7
   ch &= 0b00000111;

   // clears the bottom 3 bit before ORing
   ADMUX = (ADMUX & 0xF8) | ch;

   // Write 1 to ADSC
   ADCSRA |= (1 << ADSC);

   //Wait till ADSC becomes 0 
   while (ADCSRA & (1 << ADSC));

   return ADC;
}

int main()
{
   uint16_t adc_result;
   uint8_t led_status = 0;

   adc_init();

   while (1)
     {
        adc_result = adc_read(0); // read PA0 port 
                     // adc_read(1) // this will read port PA1
        if (adc_result < 512 && led_status == 0)
          {
             PORTB = (1 << PB0);
             led_status = 1;
          }
        else if (adc_result >= 512 && adc_result < 1024
                 && led_status == 1)
          {
             PORTB = 0;
             led_status = 0;
          }
        _delay_ms(200);
     }

   return 0;
}

