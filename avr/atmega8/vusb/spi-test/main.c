/*
 *  spi test board firmware
 *
 *  (C) Amitesh Singh <singh.amitesh@gmail.com>, 2016
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *  D2(D+) and D4(D-) are used for vusb  and PB6 and PB6 are used for 16MHz crystal
 *  then 12 GPIOs are only left for general purpose.
 *  PD0  - PD7 (excluding PD2/D+ and PD4/D - ) = 6
 *	PB0  - PB5 (excluding PB6 and PB7)  = 6
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/pgmspace.h>
#include "usbdrv.h"

#include "spi.h"

static uint8_t replybuf[3];

usbMsgLen_t
usbFunctionSetup(uchar data[8])
{
   usbRequest_t *rq = (void *)data;
   uint8_t len = 0;

   switch(rq->bRequest)
     {
      case 0:
         spi_init();
         len = 0;
         break;

      case 1:
         //read the value from HOST
         replybuf[0] = spi_send(rq->wValue.bytes[0]);

         len = 1;
         break;
         ////////////////////////////////////////////////////////////////////
      case 2:

         len = 0;
         break;

      default:
         break;
     }

   usbMsgPtr = replybuf;

   return len; // should not get here
}

int __attribute__((noreturn))
main(void)
{
   uchar i = 0;

   wdt_enable(WDTO_2S);
   usbInit();
   usbDeviceDisconnect();

   while(--i)
     {
        wdt_reset();
        _delay_ms(1);
     }

   usbDeviceConnect();
   sei();

   while(1)
     {
        wdt_reset();
        usbPoll();
     }
}
