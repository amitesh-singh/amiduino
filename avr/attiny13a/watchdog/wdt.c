#include <avr/io.h>
#include <avr/wdt.h> // Supplied Watch Dog Timer Macros 
#include <avr/interrupt.h>
#include <avr/sleep.h> // Supplied AVR Sleep Macros

#define LED_PIN PB0

/*
   The WDT is actually easier to setup than a normal Timer for the same reason it is less useful: fewer options.
   It runs on an internally calibrated 128kHz clock, meaning its timing is not effected by the main clock speed of the MCU.
   It can also continue to run during the deepest sleep modes to provide a wake up source.
*/

// As long as WDTON fuse is not programmed(not programmed by default), then you need to enable
// the WDT timeout time and then enable the watch dog timer interrupt.

/*
   From attiny13a DS,
   WDTCR – Watchdog Timer Control Registe
   
   bit 7: WDTIF: Watchdog Timer Interrupt Flag
   Bit 6 – WDTIE: Watchdog Timer Interrupt Enable
   Bit 4 – WDCE: Watchdog Change Enable
   Bit 3 – WDE: Watchdog System Reset Enable
   Bit 5, 2:0 – WDP[3:0]: Watchdog Timer Prescaler 3, 2, 1 and 0


   for setting timeout to 0.5s,
   WDTCR |= _BV(WDP0) | _BV(WDP2);

   or use helper function
   wdt_enable(WDTO_500MS);

   for 8s,
   WDP0 - 1, WDP3 - 1
   for 4:
   WDP3 - 1
   for 2:
   WDP0/1/2 - 1

   if WDTON is unprogrammed (means value is 1, on programmed, its value is 0),
   WDTON(1, unprogrammed) - WDE(0) - WDTIE(0) - Mode(Stopped), Action on timeout (None)
                          - WDE(0) - WDTIE(1) - Interrupt mode, Interrupt
                          - WDE(1) - WDTIE(0) - System Reset mode, Reset
                          - WDE(1) - WDTIE(1) - Interrupt and System Reset mode - Interrupt and then go to system reset mode.
   WDTON(0, programmed)   - WDE(x) - WDTIE(x) - System Reset mode, Reset 

   We can call wdt_reset() to not allow mcu reset.
   Reset the watchdog timer.  When the watchdog timer is enabled,
   a call to this instruction is required before the timer expires,
   otherwise a watchdog-initiated device reset will occur.

*/

ISR(WDT_vect)
{
   PORTB ^= _BV(LED_PIN);
   //wdt_reset() might require if you call wdt_enable() or enable WDE bit
}

int main()
{
   DDRB |= _BV(PB0);

   //set the prescalar to 0.5s and enable WDE as well.
   //wdt_enable(WDTO_500MS);

   //set the prescalar to 0.5s only. WDE -> 0
   WDTCR |= _BV(WDP0) | _BV(WDP2);
   //to enable 

   WDTCR |= _BV(WDTIE); //enable watchdog timer interrupt

   sei();

   while (1) {}

   return 0;
}
