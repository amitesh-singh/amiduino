#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"

const PROGMEM char usbHidReportDescriptor[52] = { /* USB report descriptor, size must match usbconfig.h */
     0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
     0x09, 0x02,                    // USAGE (Mouse)
     0xa1, 0x01,                    // COLLECTION (Application)
     0x09, 0x01,                    //   USAGE (Pointer)
     0xA1, 0x00,                    //   COLLECTION (Physical)
     0x05, 0x09,                    //     USAGE_PAGE (Button)
     0x19, 0x01,                    //     USAGE_MINIMUM
     0x29, 0x03,                    //     USAGE_MAXIMUM
     0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
     0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
     0x95, 0x03,                    //     REPORT_COUNT (3)
     0x75, 0x01,                    //     REPORT_SIZE (1)
     0x81, 0x02,                    //     INPUT (Data,Var,Abs)
     0x95, 0x01,                    //     REPORT_COUNT (1)
     0x75, 0x05,                    //     REPORT_SIZE (5)
     0x81, 0x03,                    //     INPUT (Const,Var,Abs)
     0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
     0x09, 0x30,                    //     USAGE (X)
     0x09, 0x31,                    //     USAGE (Y)
     0x09, 0x38,                    //     USAGE (Wheel)
     0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
     0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
     0x75, 0x08,                    //     REPORT_SIZE (8)
     0x95, 0x03,                    //     REPORT_COUNT (3)
     0x81, 0x06,                    //     INPUT (Data,Var,Rel)
     0xC0,                          //   END_COLLECTION
     0xC0,                          // END COLLECTION
};

typedef struct{
     uchar   buttonMask;
     char    dx;
     char    dy;
     char    dWheel;
}report_t;

static report_t reportBuffer;
static uchar    idleRate;   /* repeat rate for keyboards, never used for mice */

static void
_timer1_init()
{
   // timeout = 15ms
   TCCR0B = (1 << CS00) | ( 1 << CS02);
   // presacalar = 1024
   TCNT0 = 0;
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t    *rq = (void *)data;

   /* The following requests are never used. But since they are required by
    * the specification, we implement them in this example.
    */
   if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
     {    /* class request type */
        if(rq->bRequest == USBRQ_HID_GET_REPORT)
          {  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
             /* we only have one report type, so don't look at wValue */
             usbMsgPtr = (void *)&reportBuffer;
             return sizeof(reportBuffer);
          }
        else if(rq->bRequest == USBRQ_HID_GET_IDLE)
          {
             usbMsgPtr = &idleRate;
             return 1;
          }
        else if(rq->bRequest == USBRQ_HID_SET_IDLE)
          {
             idleRate = rq->wValue.bytes[1];
          }
     }
   else
     {
        /* no vendor specific requests implemented */
     }
   return 0;   /* default for not implemented requests: return no data back to host */
}


int __attribute__((noreturn)) main(void)
{
   int timeout = 0;
   uchar   i;
   int rand = 3765;

   wdt_enable(WDTO_1S);
   usbInit();
   usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
   i = 0;
   while(--i)
     {             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(2);
     }
   usbDeviceConnect();
   _timer1_init();
   sei();
   for(;;)
     {                /* main event loop */
        wdt_reset();
        usbPoll();
        if (TCNT0 >= 253)
          {
             timeout++;
             TCNT0 = 0;
          }
        // We are trying to get timeout if 4000 * 15 = 60 seconds
        // move the mouse after 1 min
        if(usbInterruptIsReady() && timeout > 4000)
          /* called after every poll of the interrupt endpoint */
          {
             rand = (rand * 109 + 89) % 251;
             reportBuffer.dx = (rand & 0xf) - 8;
             reportBuffer.dy = ((rand & 0xf0)>>4) - 8;

             usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
             timeout = 0;
             TCNT0 = 0;
          }
     }
}
