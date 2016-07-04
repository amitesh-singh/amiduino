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

static uchar buffer[64];
const char driver_name[] PROGMEM = "vusb based usb 1.1 device driver";
const char driver_ver[] PROGMEM = "0.1";
void copyString(unsigned char *buf, const char *buf2, int len)
{
   uint8_t i  = 0;
   for (; i < len; ++i)
     {
        buf[i] = pgm_read_byte(&buf2[i]);
     }
}

//This is where custom message is handled from HOST
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t *rq = (void *)data; // cast data to correct type
   usbMsgLen_t len = 64;

   // custom command is in the bRequest field
   switch(rq->bRequest)
     {
         case USB_DRIVER_NAME:
            if (len > rq->wLength.word)
              len = rq->wLength.word;
            copyString(buffer, driver_name, sizeof(driver_name)/sizeof(char));
            //strcpy((char *)buffer, "vusb based usb 1.1");
            usbMsgPtr = buffer;
            break;
         case USB_DRIVER_VER:
            if (len > rq->wLength.word)
              len = rq->wLength.word;
            copyString(buffer, driver_ver, sizeof(driver_ver)/sizeof(char));
            //strcpy((char *)buffer, "0.1");
            usbMsgPtr = buffer;
            break;
         case USB_DRIVER_AUTH:
            if (len > rq->wLength.word)
              len = rq->wLength.word;
            //strcpy((char *) buffer, "Amitesh Singh");
            //The below code works.
            // This is for one time usage only.
            strcpy_P((char *)buffer, PSTR("Amitesh Singh"));
            usbMsgPtr = buffer;
            break;
         default:
            return 0;// should not get here
     }

   return len;
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
