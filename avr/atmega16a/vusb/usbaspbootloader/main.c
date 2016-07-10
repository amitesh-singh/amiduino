/*
   Atmega16A
   ----------
   INT0 - PD2
   connect D+ (without any caps, 100 pf(104) was creating issues) to INT0 (PD2)
   connect D- (without any caps, 100 pf (104) was creating issues) to PD4
*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/boot.h> //for boot related functions
#include "usbdrv.h"

#define USBASP_CONNECT     1
#define USBASP_DISCONNECT  2 // we are not handling this case.
#define USBASP_TRANSMIT    3 // this is for giving information like device sign, lfuse, hfuse, efuse 
#define USBASP_READFLASH   4 // this reads the whole flash

void (*jump_to_app)(void) = 0x0000;

void leave_bootloader()
{
   wdt_disable();
   //better than boot_rww_enable()
   boot_rww_enable_safe(); //enable the rww region and wait for job to be finished
   //boot_rww_enable(); //enable the rww region
   GICR  = (1 << IVCE); // enable change of interrupt vectors
   GICR = (0 << IVSEL); // move interrupts to application flash section
// asm("jmp 0x0000"); -- this should also work
   jump_to_app();
}

#define STATE_IDLE 0
#define STATE_WRITE 1
#define STATE_READ 2

static int8_t state = STATE_IDLE;
static unsigned int page_address;
static uchar replyBuf[4];


static int bytes_remaining = 0;
static uchar is_last_page = 0;
static int8_t isbootloader_exit = 0;
static uchar device_signature[] = {0x1e, 0x94, 0x03};
//This is where custom message is handled from HOST
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t *rq = (void *)data; // cast data to correct type
   uchar rval = 0;

   switch(rq->bRequest)
     { // custom command is in the bRequest field
        //atmega16a SPM_PAGESIZE is  128 bytes
      case USBASP_TRANSMIT:
         replyBuf[1] = rq->wValue.bytes[0];
         replyBuf[2] = rq->wValue.bytes[1];
         if (rq->wValue.bytes[0] == 0x30)
           {
              rval = rq->wIndex.bytes[0] & 3;
              replyBuf[3] = device_signature[rval];
           }
         else if (rq->wValue.bytes[0] == 0x58 && rq->wValue.bytes[1] == 0x00)
           {
              //lock fuse
              replyBuf[3] = boot_lock_fuse_bits_get(GET_LOCK_BITS);
           }
         else if (rq->wValue.bytes[0] == 0x50 && rq->wValue.bytes[1] == 0x00)
           {
              replyBuf[3] = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
           }
         else if (rq->wValue.bytes[0] == 0x58 && rq->wValue.bytes[1] == 0x08)
           {
              replyBuf[3] = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
           }
         usbMsgPtr = replyBuf;
         return 4;
      case USBASP_READFLASH:
         state = STATE_READ;
         page_address = rq->wValue.word;
         bytes_remaining = rq->wLength.bytes[0];
         is_last_page = rq->wIndex.bytes[1] & 0x02;

         return USB_NO_MSG;

         //replyBuf[0] = SPM_PAGESIZE >> 8;
         //replyBuf[1] = SPM_PAGESIZE & 0xff;
         //usbMsgPtr = replyBuf;
         return 2;
      case USBASP_DISCONNECT:
        // isbootloader_exit = 1; // exit gracefully
         return 0;
     }
   usbMsgPtr = replyBuf;

   return 0; // should not get here
}

uchar usbFunctionRead(uchar *data, uchar len)
{
  uchar i = 0;
  if (len > bytes_remaining)
    len = bytes_remaining;
  bytes_remaining -= len;

  for (; i < len; ++i)
  {
    //only reading flash section
    data[i] = pgm_read_byte(page_address++);
  }

  return i;
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
        if (isbootloader_exit)
          {
             usbDeviceDisconnect();
             leave_bootloader();
          }
     }
}
