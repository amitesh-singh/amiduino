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


#define USB_DATA_IN 4 // this is to send big data to device
#define USB_DATA_WRITE 3
#define USB_DATA_OUT 2
#define USB_LED_ON 1
#define USB_LED_OFF 0

//static data stored
static unsigned char replyBuf[] = "Hello Ami";
//This is where custom message is handled from HOST
static unsigned char dataRecieved = 0, dataLength = 0;
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
         //Send data to Host from device
      case USB_DATA_OUT:
         usbMsgPtr = replyBuf;
         return sizeof(replyBuf);
         //Send data from device to host
      case USB_DATA_WRITE:
         replyBuf[7] = rq->wValue.bytes[0];
         replyBuf[8] = rq->wValue.bytes[1];
         replyBuf[9] = rq->wIndex.bytes[0];

         return 0;
         //If we want to send more data to device, then use the below technique
      case USB_DATA_IN:
         dataLength = (uchar)rq->wLength.word;
         dataRecieved = 0;
         if (dataLength > sizeof(replyBuf))
           dataLength = sizeof(replyBuf);

         return USB_NO_MSG; //this macro value is 255
         // now we need to implement usbFunctionWrite(), make sure u define #define USB_CFG_IMPLEMENT_FN_WRITE      1 in usbconfig.h

     }

   return 0; // should not get here
}

USB_PUBLIC uchar usbFunctionWrite(uchar *data, uchar len)
{
   uchar i;

   for (i = 0; i <len && dataRecieved < dataLength; ++i, ++dataRecieved)
   {
      replyBuf[dataRecieved] = data[i];
   }

   return (dataRecieved == dataLength); //return 1 if we recieved all data else return 0
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
