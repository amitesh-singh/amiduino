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

#include <avr/boot.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */

#define GET_PAGE_SIZE 1
#define LEAVE_BOOTLOADER 2
#define UPLOAD_NEW_PROGRAM 3

void (*jump_to_app)(void) = 0x0000;

void leave_bootloader()
{
   uchar temp;
   cli();
   boot_rww_enable(); //enable the rww region
   temp = GICR;
   GICR = temp | (1 << IVCE); // enable change of interrupt vectors
   GICR = temp & ~(1 << IVSEL); // move interrupts to application flash section
//   asm("jmp 0x0000");
   jump_to_app();
}

#define STATE_IDLE 0
#define STATE_WRITE 1
static int8_t state = STATE_IDLE;
static unsigned int page_address;
static unsigned int page_offset;
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
      case LEAVE_BOOTLOADER:
         //We are leaving bootloader.. disconnect usb
         usbDeviceDisconnect();
         leave_bootloader();
         break;
      case UPLOAD_NEW_PROGRAM:
         state = STATE_WRITE;

         page_address = (data[3] << 8) | (data[2]);
         page_offset = 0;

         eeprom_busy_wait();
         cli();
         boot_page_erase(page_address);
         sei();
         boot_spm_busy_wait(); //wait until page is erased
         return USB_NO_MSG; //usbfunctionwrite
     }

   return 0; // should not get here
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
   uchar i;
   if (state != STATE_WRITE)
     return USB_NO_MSG;

   for (i = 0; i < len; i+=2)
     {
        cli();
        boot_page_fill(page_address + page_offset, data[i] | data[i + 1] << 8);
        sei();
        page_offset += 2;
        if (page_offset >= SPM_PAGESIZE)
          {
             cli();
             boot_page_write(page_address);
             sei();
             boot_spm_busy_wait();
             state = STATE_IDLE;
             return 1; //done with writing
          }
     }

   return 0;
}

int __attribute__((noreturn)) main(void)
{
   uchar   i;

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
