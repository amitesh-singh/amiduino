/*
  AVRUSBBoot - USB bootloader for Atmel AVR controllers

  Thomas Fischl <tfischl@gmx.de>

  License:
  The project is built with AVR USB driver by Objective Development, which is
  published under a proprietary Open Source license. To conform with this
  license, USBasp is distributed under the same license conditions. See
  documentation.

  Target.........: ATMega8 at 12 MHz
  Creation Date..: 2006-03-18
  Last change....: 2006-06-25

  To adapt the bootloader to your hardware, you have to modify the following files:
  - bootloaderconfig.h:
    Define the condition when the bootloader should be started
  - usbconfig.h:
    Define the used data line pins. You have to adapt USB_CFG_IOPORT, USB_CFG_DMINUS_BIT and 
    USB_CFG_DPLUS_BIT to your hardware. The rest should be left unchanged.
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <util/delay.h>

#include "usbdrv.h"
#include "bootloaderconfig.h"

#define USBBOOT_FUNC_WRITE_PAGE 2
#define USBBOOT_FUNC_LEAVE_BOOT 1
#define USBBOOT_FUNC_GET_PAGESIZE 3

#define STATE_IDLE 0
#define STATE_WRITE_PAGE 1


#define DLED_ON {  PORTB &= ~(1<<PB0); DDRB &= ~(1<<PB0); }
#define DLED_OFF { PORTB &= ~(1<<PB0); DDRB |=  (1<<PB0); }

static uchar replyBuffer[8];
static uchar state = STATE_IDLE;
static unsigned int page_address;
static unsigned int page_offset;

void (*jump_to_app)(void) = 0x0000;

void leaveBootloader() {
      cli();
      boot_rww_enable();
      GICR = (1 << IVCE);  /* enable change of interrupt vectors */
      GICR = (0 << IVSEL); /* move interrupts to application flash section */
      jump_to_app();
}

uchar   usbFunctionSetup(uchar data[8])
{
    uchar len = 0;
    
    if (data[1] == USBBOOT_FUNC_LEAVE_BOOT) {
		usbDeviceDisconnect();
      	leaveBootloader();
    } 
    else if (data[1] == USBBOOT_FUNC_WRITE_PAGE) {

      state = STATE_WRITE_PAGE;

      page_address = (data[3] << 8) | data[2]; /* page address */
      page_offset = 0;

      eeprom_busy_wait();
      cli();
      boot_page_erase(page_address); /* erase page */
      sei();
      boot_spm_busy_wait(); /* wait until page is erased */

      len = 0xff; /* multiple out */

    } 

    else if (data[1] == USBBOOT_FUNC_GET_PAGESIZE) {

      replyBuffer[0] = SPM_PAGESIZE >> 8;
      replyBuffer[1] = SPM_PAGESIZE & 0xff;
      len = 2;

    }

    usbMsgPtr = replyBuffer;

    return len;
}


uchar usbFunctionWrite(uchar *data, uchar len)
{

  uchar i;

  /* check if we are in correct state */
  if (state != STATE_WRITE_PAGE)
    return 0xff;
  
  for (i = 0; i < len; i+=2) {

    cli();    
    boot_page_fill(page_address + page_offset, data[i] | (data[i + 1] << 8));
    sei();
    page_offset += 2;

    /* check if we are at the end of a page */
    if (page_offset >= SPM_PAGESIZE) {
      
      /* write page */
      cli();
      boot_page_write(page_address);
      sei();
      boot_spm_busy_wait();

      state = STATE_IDLE;
      return 1;
    }

  }
  
  return 0;
}

int main(void)
{
   BOOTLOADER_INIT;
   DLED_OFF;
   _delay_ms(100);
   DLED_ON;
   _delay_ms(100);
   DLED_OFF;
   _delay_ms(100);
   DLED_ON;

   /* initialize hardware */

   /* jump to application if jumper is set */
   if (!BOOTLOADER_CONDITION) {
        leaveBootloader();
   }

   GICR = (1 << IVCE);  /* enable change of interrupt vectors */
   GICR = (1 << IVSEL); /* move interrupts to boot flash section */

   wdt_enable(WDTO_1S);
   usbInit();
   usbDeviceDisconnect();
   uchar i = 0;
   while (--i)
     {
        wdt_reset();
        _delay_ms(1);
     }
   usbDeviceConnect();
   sei();
   for(;;)
     {    /* main event loop */
        wdt_reset();
        usbPoll();
     }
   return 0;
}

