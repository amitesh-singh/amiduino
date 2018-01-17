#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
//attiny13a consumption is only 5uA in sleepmode.

ISR(WDT_vect)
{
}

static void
save_more_power()
{
   //without this below code, the current consumption is around 0.25mA
   //turn off ADC
   ADCSRA &= ~(1 << ADEN); // this gives 0.005mA at 3.3v
   ACSR = (1<<ACD); //Disable the analog comparator
   DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins.
}

int main(void)
{
   uart_puts("Sleep mode example.\r\n");
   save_more_power();

   //set the prescalar to 8s 
   WDTCR |=  (1 << WDP3 )|(1 << WDP0);
   WDTCR |= _BV(WDTIE); //enable watchdog timer interrupt
   sei();

   while (1)
     {
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode(); // <-- sleeps here
        //wokes up after 8s and print this message.
        uart_puts("wokeup\r\n");
     }

   return 0;
}
