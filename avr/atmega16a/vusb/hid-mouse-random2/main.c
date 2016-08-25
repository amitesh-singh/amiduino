// mouse firmware code to which randomize the mouse cursor position on 
// every 15 seconds
// Written by Amitesh Singh 2016
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/pgmspace.h>
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

typedef struct {
     uchar   buttonMask;
     char    dx;
     char    dy;
     char    dWheel;
}report_t;

static report_t reportBuffer;
static uchar    idleRate;

usbMsgLen_t
usbFunctionSetup(uchar data[8])
{
   usbRequest_t    *rq = (void *)data;

   if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
     {
        if(rq->bRequest == USBRQ_HID_GET_REPORT)
          {
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
     }
   return 0;
}

volatile int overflows = 0;
ISR(TIMER0_COMP_vect)
{
   overflows++;
}

static void
_timer0_init()
{
   TCCR0 = (1 << CS00) | (1 << CS02); //prescalar = 1024
   TCNT0 = 0;
   OCR0 = 245;

   TIMSK |= (1 << OCIE0); // enable timer comp interrupt
}

int __attribute__((noreturn))
main(void)
{
   uint8_t i = 0;
   int rand = 3765;
   int8_t move_mouse = 0;

   wdt_enable(WDTO_1S);
   usbInit();
   usbDeviceDisconnect();
   while(--i)
     {
        wdt_reset();
        _delay_ms(1);
     }
   usbDeviceConnect();
   //start the timer
   _timer0_init();
   sei();

   for(;;)
     {
        wdt_reset();
        usbPoll();

        if (overflows > 100)
          {
             overflows = 0;
             move_mouse = 1;
          }

        if(usbInterruptIsReady() && move_mouse == 1)
          {
             rand = (rand * 109 + 89) % 251;
             reportBuffer.dx = (rand & 0xf) - 8;
             reportBuffer.dy = ((rand & 0xf0)>>4) - 8;

             usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
             move_mouse = 0;
          }
     }
}
