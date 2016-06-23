#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usbdrv.h"

#include <util/delay.h>

#define USB_LED_OFF 0
#define USB_LED_ON  1

// this gets called when custom control message is received
USB_PUBLIC uchar usbFunctionSetup(uchar data[8])
{
   /*
      typedef struct usbRequest
      {
         uchar       bmRequestType;
         uchar       bRequest;
         usbWord_t   wValue;
         usbWord_t   wIndex;
         usbWord_t   wLength;
      }usbRequest_t;
    */
   usbRequest_t *rq = (void *)data; // cast data to correct type

   switch(rq->bRequest)
     { // custom command is in the bRequest field
      case USB_LED_ON:
         PORTB |= (1 << PB0); // turn LED on
         return 0;
      case USB_LED_OFF:
         PORTB &= ~(1 << PB0); // turn LED off
         return 0;
     }

   return 0; // should not get here
}

int main()
{
   int8_t i;

   DDRB |= (1 << PB0); // PB0 as output

   wdt_enable(WDTO_1S); // enable 1s watchdog timer

   usbInit();

   usbDeviceDisconnect(); // enforce re-enumeration

   for(i = 0; i<250; i++)
     { // wait 500 ms
        wdt_reset(); // keep the watchdog happy
        _delay_ms(2);
     }
   usbDeviceConnect();

   sei(); // Enable interrupts after re-enumeration

   while(1)
     {
        wdt_reset(); // keep the watchdog happy
        usbPoll();
     }

   return 0;
}
