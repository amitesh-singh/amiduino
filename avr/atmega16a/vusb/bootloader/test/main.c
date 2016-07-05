#ifndef F_CPU
#define F_CPU 20000000L
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

#define DLED_ON {  PORTB &= ~(1<<PB0); DDRB &= ~(1<<PB0); }
#define DLED_OFF { PORTB &= ~(1<<PB0); DDRB |=  (1<<PB0); }

#define START_ADC_MEASUREMENT() { ADCSRA |= (1 << ADSC);            }
#define WAIT_FOR_ADC_VALUE()    { while( ADCSRA & (1 << ADSC) ) { } }


uint8_t get_random_num(void)
{
 uint8_t i=0,dummy1=0,dummy2=0,result=0;
 
 for(i=0;i<8;i++)
  {
   START_ADC_MEASUREMENT();
   WAIT_FOR_ADC_VALUE();
   dummy1 = ADCL;
   dummy2 = ADCH;
   
   dummy2 = dummy1 & 0x01;
   
   if(dummy2 > 0)
    result |= (1<<i);
    
  }
  return result;
}


//FUNCTION "main"
int main (void) 
{
char i, dummy;
DDRC=0x00;
PORTC=0x00;


uart_init();
uart_puts("booting...\r\n");

// ADC0 Init
ADMUX  |= (1 << REFS0);                            // AVcc with external cap at AREF Pin
ADCSRA |= ((1 << ADEN) | (1 << ADPS2) | (1 << ADPS1));              // ADC enable, Prescaler 64
  
// make one first measurment
ADCSRA |= (1 << ADSC);         // ADC Start Conversion
WAIT_FOR_ADC_VALUE();

dummy = ADCL;
dummy = ADCH;

dummy=0;

while(1){

	for(i=0; i<10; i++){
		DLED_ON;
		_delay_ms(20);
		DLED_OFF;
		_delay_ms(get_random_num()*2);
	}

/*	for(i=0; i<5; i++){
		DLED_ON;
		_delay_ms(50);
		DLED_OFF;
		_delay_ms(250);
	}
*/	

}
return 0;
}
