/*

   connect D+ (without any caps, 100 pf(104) was creating issues) to INT0 (PB2)
   connect D- (without any caps, 100 pf (104) was creating issues) to PB0
   connect a LED with resistor on PB1 to Ground

*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */

#define USB_LED_ON 1
#define USB_LED_OFF 0

//This is where custom message is handled from HOST
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t *rq = (void *)data; // cast data to correct type

   switch(rq->bRequest)
     { // custom command is in the bRequest field
      case USB_LED_ON:
         PORTB |= (1 << PB1); // turn LED on
         return 0;
      case USB_LED_OFF:
         PORTB &= ~(1 << PB1); // turn LED off
         return 0;
     }

   return 0; // should not get here
}


int __attribute__((noreturn)) main(void)
{
   uchar   i;
   DDRB |= (1 << PB1); //define DDRB before doing usb stuffs

   wdt_enable(WDTO_1S);
   /* Even if you don't use the watchdog, turn it off here. On newer devices,
    * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!
    */
   /* RESET status: all port bits are inputs without pull-up.
    * That's the way we need D+ and D-. Therefore we don't need any
    * additional hardware initialization.
    */
   odDebugInit();
   DBG1(0x00, 0, 0);       /* debug output: main starts */
   usbInit();
   usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
   i = 0;
   while(--i){             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
   }
   usbDeviceConnect();
   sei();
   DBG1(0x01, 0, 0);       /* debug output: main loop starts */
   for(;;){                /* main event loop */
        DBG1(0x02, 0, 0);   /* debug output: main loop iterates */
        wdt_reset();
        usbPoll();
   }
}
