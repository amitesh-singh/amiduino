
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"

#define USB_LEFT_MOVE 5
#define USB_RIGHT_MOVE 6
#define USB_UP_MOVE 7
#define USB_DOWN_MOVE 8

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
/* This is the same report descriptor as seen in a Logitech mouse. The data
 * described by this descriptor consists of 4 bytes:
 *      .  .  .  .  . B2 B1 B0 .... one byte with mouse button states
 *     X7 X6 X5 X4 X3 X2 X1 X0 .... 8 bit signed relative coordinate x
 *     Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0 .... 8 bit signed relative coordinate y
 *     W7 W6 W5 W4 W3 W2 W1 W0 .... 8 bit signed relative coordinate wheel
 */
typedef struct{
     uchar   buttonMask;
     char    dx;
     char    dy;
     char    dWheel;
}report_t;

static report_t reportBuffer;
static uchar    idleRate;   /* repeat rate for keyboards, never used for mice */

/* ------------------------------------------------------------------------- */

volatile int updated = 0;
volatile int x = 0;
volatile int y = 0;
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t    *rq = (void *)data;

   /* The following requests are never used. But since they are required by
    * the specification, we implement them in this example.
    */
   if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
     {    /* class request type */
        DBG1(0x50, &rq->bRequest, 1);   /* debug output: print our request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
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
        switch(rq->bRequest)
          {
           case USB_LEFT_MOVE:
              x = -10;
              y = 0;
              updated = 1;
              return 0;
           case USB_RIGHT_MOVE:
              x = +10;
              y = 0;
              updated = 1;
              return 0;
           case USB_UP_MOVE:
              x = 0;
              y = -10;
              updated = 1;
              return 0;
           case USB_DOWN_MOVE:
              x = 0;
              y = 10;
              updated = 1;
              return 0;
          }
        /* no vendor specific requests implemented */
     }
   return 0;   /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */

int __attribute__((noreturn)) main(void)
{
   uchar   i;
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
   while(--i)
     {             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
     }
   usbDeviceConnect();
   sei();
   DBG1(0x01, 0, 0);       /* debug output: main loop starts */
   while(1)
     {                /* main event loop */
        DBG1(0x02, 0, 0);   /* debug output: main loop iterates */
        wdt_reset();
        usbPoll();
        // This was resulting into timeout issue
        //_delay_ms(100);
        if(usbInterruptIsReady() && updated == 1)
          /* called after every poll of the interrupt endpoint */
          {
             reportBuffer.dx += x;
             reportBuffer.dy += y;
             x = 0; y = 0;
             usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
             updated = 0;
          }
     }
}
