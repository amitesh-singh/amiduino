/*
   Atmega16A
   ----------
   INT0 - PD2
   connect D+ (without any caps, 100 pf(104) was creating issues) to INT0 (PD2)
   connect D- (without any caps, 100 pf (104) was creating issues) to PD0

*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */

#define GET_PAGE_SIZE 1

static uchar replyBuf[8];
//This is where custom message is handled from HOST
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t *rq = (void *)data; // cast data to correct type

   switch(rq->bRequest)
     { // custom command is in the bRequest field
      case GET_PAGE_SIZE:
         replyBuf[0] = SPM_PAGESIZE >> 8;
         replyBuf[1] = SPM_PAGESIZE & 0xff;
         usbMsgPtr = replyBuf;
         return 2;
     }

   return 0; // should not get here
}

int __attribute__((noreturn)) main(void)
{
   uchar   i;
   DDRB |= (1 << PB1); //define DDRB before doing usb stuffs

   //This is important or else bootloader won't get loaded
   GICR = (1 << IVCE);  /* enable change of interrupt vectors */
   GICR = (1 << IVSEL); /* move interrupts to boot flash section */

   wdt_enable(WDTO_1S);
   usbInit();
   usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
   i = 0;
   while(--i)
     {             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
     }
   usbDeviceConnect();
   sei();
   while(1)
     {                /* main event loop */
        wdt_reset();
        usbPoll();
   }
}
