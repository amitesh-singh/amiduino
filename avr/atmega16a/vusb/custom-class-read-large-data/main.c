/*
   Atmega16A
   ----------
   INT0 - PD2
   connect D+ (without any caps, 100 pf(104) was creating issues) to INT0 (PD2)
   connect D- (without any caps, 100 pf (104) was creating issues) to PD0

   An example of usbFunctionRead()

*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <string.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */

#define USB_DRIVER_NAME 1
#define USB_DRIVER_VER 2
#define USB_DRIVER_AUTH 3

static int dataSent  = 0;
//This is where custom message is handled from HOST
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t *rq = (void *)data; // cast data to correct type

   // custom command is in the bRequest field
   switch(rq->bRequest)
     {
         case USB_DRIVER_NAME:
            dataSent = 0;
            break;
         case USB_DRIVER_VER:
            dataSent = 0;
            break;
         case USB_DRIVER_AUTH:
            dataSent = 0;
            break;
         default:
            return 0;// should not get here
     }

   return USB_NO_MSG;
}

//send data to PC upto 1024 bytes
USB_PUBLIC uchar usbFunctionRead(uchar *data, uchar len)
{
   uchar i = 0;
   for (; dataSent < 1024 && i < len; ++i, dataSent++)
     {
        data[i] = '0' + i;
     }
   if (i && dataSent == 1024)
     data[i - 1] = 0;

   return i;
}

int __attribute__((noreturn)) main(void)
{
   uchar   i;
   DDRB |= (1 << PB1); //define DDRB before doing usb stuffs

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
