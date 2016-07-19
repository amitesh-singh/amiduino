/*
   Atmega16A - vusb based bootloader for atmega16a
   ----------
   INT0 - PD2
   connect D+ (without any caps, 100 pf(104) was creating issues) to INT0 (PD2)
   connect D- (without any caps, 100 pf (104) was creating issues) to PD4
   how to use:
   avrdude -c usbasp-clone -p atmega16 
   avrdude -c usbasp-clone -p atmega16 -U flash:w:somebinary.hex
   Author: Amitesh Singh <singh.amitesh at gmail.com> Copyright 2016
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
#define USBASP_ENABLEPROG  5 // enable th prog, we do need to return 0 with len = 1
#define USBASP_SETISPSCK   10 // this sets the speed of spi. we don't bother abt it. I added this to shut 
                             // the annoying message i got from avrdude.
#define USBASP_WRITEFLASH   6 // write to flash - into app section

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

static unsigned int page_address;
static uchar page_offset;
//static uchar isLastPage = 0;
static uchar replyBuf[4];

static uchar isbootloader_exit = 0;

static int bytes_remaining;
//This is where custom message is handled from HOST
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   static uchar device_signature[] = {0x1e, 0x94, 0x03};
   usbRequest_t *rq = (void *)data; // cast data to correct type
   uchar len = 0;

   switch(rq->bRequest)
     { // custom command is in the bRequest field
        //atmega16a SPM_PAGESIZE is  128 bytes
      case USBASP_ENABLEPROG: // USBASP_ENABLPROG
      case USBASP_SETISPSCK: // USBASP_FUNC_SETISPSCK 10
         // This fixes the annoying message i got avrdude: warning: cannot set sck period. please check for usbasp firmware update.
         // there is no spi speed concept here, we are just emulating the usbasp programmer into our
         // bootloader, we really don't bother. so just satisfy avrdude by sending a fake reply. ;P
         len = 1;
         break;

      case USBASP_TRANSMIT:
         replyBuf[1] = rq->wValue.bytes[0];
         replyBuf[2] = rq->wValue.bytes[1];
         if (rq->wValue.bytes[0] == 0x30)
           {
              replyBuf[3] = device_signature[rq->wIndex.bytes[0] & 3];
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
         len = 4;
         break;
         //TODO: make read code smaller, write everything in asm
         // macro for read function is disabled as the size was crossing > 2k
         /*
      case USBASP_READFLASH:
         page_address = rq->wValue.word;
         bytes_remaining = rq->wLength.bytes[0];
         is_last_page = rq->wIndex.bytes[1] & 0x02;

         len = USB_NO_MSG;
         break;
         */

      case USBASP_WRITEFLASH:
         // USBAP_FUNC_WRITEFLASH: byte1, byte2, byte3, byte4
         // byte1, byte2 => page address
         // byte3 - bytes to be sent
         // byte4 - always 0x01
         // isLastPage = in my debug, avrdude always sent 0x01
         // so i don't think its needed
         page_address = rq->wValue.word;
         //isLastPage = rq->wIndex.bytes[1] & 0x02;
         bytes_remaining =  rq->wIndex.bytes[0];
         page_offset = 0;

         /*
         //this is not required since we are already overwriting page data
         eeprom_busy_wait();
         cli();
         boot_page_erase(page_address);
         sei();
         boot_spm_busy_wait(); //wait until page is erased
         */
         len = USB_NO_MSG;

         break;
         //replyBuf[0] = SPM_PAGESIZE >> 8;
         //replyBuf[1] = SPM_PAGESIZE & 0xff;
         //usbMsgPtr = replyBuf;
         //return 2;
      case USBASP_DISCONNECT:
         //usbDeviceDisconnect();
         //leave_bootloader();
         isbootloader_exit = 1; // exit gracefully
         //return 0;
         break;
     }
   usbMsgPtr = replyBuf;

   return len; // should not get here
}

/*
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
*/

uchar usbFunctionWrite(uchar *data, uchar len)
{
   uchar i= 0, isLast;
   if (len > bytes_remaining)
     len = bytes_remaining;
   bytes_remaining -= len;
   isLast = bytes_remaining == 0;

   for (; i < len; i+=2)
     {
        cli();
        boot_page_fill(page_address + page_offset, data[i] | data[i + 1] << 8);
        sei();
        page_offset += 2;
        if (page_offset >= SPM_PAGESIZE || (isLast && i >= len /*&& isLastPage*/))
          {
             cli();
             boot_page_write(page_address);
             sei();
             boot_spm_busy_wait();
             return 1; //done with writing
          }
     }

   return 0;
}

int main(void)
{
   uchar   i = 0;

   //This is important or else bootloader won't get loaded
   GICR = (1 << IVCE);  /* enable change of interrupt vectors */
   GICR = (1 << IVSEL); /* move interrupts to boot flash section */

   wdt_enable(WDTO_1S);
   usbInit();
   usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
   while(--i)
     {             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(2);
     }
   usbDeviceConnect();
   sei();
   while(1)
     {                /* main event loop */
        wdt_reset();
        usbPoll();
        if (isbootloader_exit)
          {
             break;
          }

     }

   wdt_disable();
   usbDeviceDisconnect();
   //give a delay for 100ms is required
   _delay_ms(400);
   leave_bootloader();

   return 0;
}
