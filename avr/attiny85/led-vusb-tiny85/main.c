#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usbdrv.h"

#include <util/delay.h>
#include <avr/eeprom.h>

#define USB_LED_OFF 0
#define USB_LED_ON  1

static void calibrateOscillator(void)
{
   uchar       step = 128;
   uchar       trialValue = 0, optimumValue;
   int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

   /* do a binary search: */
   do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    // proportional to current real frequency
        if(x < targetValue)             // frequency still too low
          trialValue += step;
        step >>= 1;
   }while(step > 0);
   /* We have a precision of +/- 1 for optimum OSCCAL here */
   /* now do a neighborhood search for optimum value */
   optimumValue = trialValue;
   optimumDev = x; // this is certainly far away from optimum
   for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
          x = -x;
        if(x < optimumDev){
             optimumDev = x;
             optimumValue = OSCCAL;
        }
   }
   OSCCAL = optimumValue;
}

void    usbEventResetReady(void)
{
   cli();  // usbMeasureFrameLength() counts CPU cycles, so disable interrupts.
   calibrateOscillator();
   sei();
   eeprom_write_byte(0, OSCCAL);   // store the calibrated value in EEPROM
}

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
