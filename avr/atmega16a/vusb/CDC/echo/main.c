/*
   Atmega16A
   ----------
   INT0 - PD2
   connect D+ (without any caps, 100 pf(104) was creating issues) to INT0 (PD2)
   connect D- (without any caps, 100 pf (104) was creating issues) to PD0
   connect a LED with resistor on PB1 to Ground

*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <string.h> //for memset

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */

#define USB_LED_ON 1
#define USB_LED_OFF 0

#define HIDSERIAL_INBUFFER_SIZE 32


static uchar received = 0;
static uchar outBuffer[8];
static uchar inBuffer[HIDSERIAL_INBUFFER_SIZE];
static uchar reportId = 0;
static uchar bytesRemaining;
static uchar* pos;
static PROGMEM const char configDescrCDC[] = {   /* USB configuration descriptor */
    9,          /* sizeof(usbDescrConfig): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    67,
    0,          /* total length of data returned (including inlined descriptors) */
    2,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
#if USB_CFG_IS_SELF_POWERED
    (1 << 7) | USBATTR_SELFPOWER,       /* attributes */
#else
    (1 << 7),                           /* attributes */
#endif
    USB_CFG_MAX_BUS_POWER/2,            /* max USB current in 2mA units */

    /* interface descriptor follows inline: */

    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    0,          /* index of this interface */
    0,          /* alternate setting for this interface */
    USB_CFG_HAVE_INTRIN_ENDPOINT,   /* endpoints excl 0: number of endpoint descriptors to follow */
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    0,          /* string index for interface */

    /* CDC Class-Specific descriptor */

    5,           /* sizeof(usbDescrCDC_HeaderFn): length of descriptor in bytes */
    0x24,        /* descriptor type */
    0,           /* header functional descriptor */
    0x10, 0x01,

    4,           /* sizeof(usbDescrCDC_AcmFn): length of descriptor in bytes */
    0x24,        /* descriptor type */
    2,           /* abstract control management functional descriptor */
    0x02,        /* SET_LINE_CODING,    GET_LINE_CODING, SET_CONTROL_LINE_STATE    */
    5,           /* sizeof(usbDescrCDC_UnionFn): length of descriptor in bytes */
    0x24,        /* descriptor type */
    6,           /* union functional descriptor */
    0,           /* CDC_COMM_INTF_ID */
    1,           /* CDC_DATA_INTF_ID */
    5,           /* sizeof(usbDescrCDC_CallMgtFn): length of descriptor in bytes */
    0x24,        /* descriptor type */
    1,           /* call management functional descriptor */
    3,           /* allow management on data interface, handles call management by itself */
    1,           /* CDC_DATA_INTF_ID */

    /* Endpoint Descriptor */

    7,           /* sizeof(usbDescrEndpoint) */

    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x80|USB_CFG_EP3_NUMBER,        /* IN endpoint number */
    0x03,        /* attrib: Interrupt endpoint */
    8, 0,        /* maximum packet size */
    USB_CFG_INTR_POLL_INTERVAL,        /* in ms */
    /* Interface Descriptor  */
    9,           /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE,           /* descriptor type */
    1,           /* index of this interface */
    0,           /* alternate setting for this interface */
    2,           /* endpoints excl 0: number of endpoint descriptors to follow */
    0x0A,        /* Data Interface Class Codes */
    0,
    0,           /* Data Interface Class Protocol Codes */
    0,           /* string index for interface */

    /* Endpoint Descriptor */

    7,           /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x01,        /* OUT endpoint number 1 */
    0x02,        /* attrib: Bulk endpoint */
    8, 0,        /* maximum packet size */
    0,           /* in ms */

    /* Endpoint Descriptor */

    7,           /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x81,        /* IN endpoint number 1 */
    0x02,        /* attrib: Bulk endpoint */
    8, 0,        /* maximum packet size */
    0,           /* in ms */
};

uchar usbFunctionDescriptor(usbRequest_t *rq)
{
   if(rq->wValue.bytes[1] == USBDESCR_DEVICE){
        usbMsgPtr = (uchar *)usbDescriptorDevice;
        return usbDescriptorDevice[0];
   }else{  /* must be config descriptor */
        usbMsgPtr = (uchar *)configDescrCDC;
        return sizeof(configDescrCDC);
   }
}

/* usbFunctionRead() is called when the host requests a chunk of data from
 * the device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionRead(uchar *data, uchar len)
{
    return 0;
}

/* usbFunctionWrite() is called when the host sends a chunk of data to the
 * device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if (reportId == 0) {
        int i;
        if(len > bytesRemaining)
            len = bytesRemaining;
        bytesRemaining -= len;
        //int start = (pos==inBuffer)?1:0;
        for(i=0;i<len;i++) {
            if (data[i]!=0) {
                *pos++ = data[i];
             }
        }
        if (bytesRemaining == 0) {
            received = 1;
            *pos++ = 0;
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}
//This is where custom message is handled from HOST
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t    *rq = (usbRequest_t *)data;
   reportId = rq->wValue.bytes[0];
   if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){
             /* wValue: ReportType (highbyte), ReportID (lowbyte) */
             /* since we have only one report type, we can ignore the report-ID */
             return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
             /* since we have only one report type, we can ignore the report-ID */
             pos = inBuffer;
             bytesRemaining = rq->wLength.word;
             if(bytesRemaining > sizeof(inBuffer))
               bytesRemaining = sizeof(inBuffer);
             return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
        }
   }else{
        /* ignore vendor type requests, we don't use any */
   }
   return 0;
}

static int
is_available()
{
   return received;
}

static uchar
readchar(uchar *buffer)
{
   if(received == 0) return 0;
   int i;
   for(i=0;inBuffer[i]!=0&&i<HIDSERIAL_INBUFFER_SIZE;i++)
     {
        buffer[i] = inBuffer[i];
     }
   inBuffer[0] = 0;
   buffer[i] = 0;
   received = 0;
   return i;
}

// write one character
static size_t
writechar(uint8_t data)
{
   while(!usbInterruptIsReady()) {
        usbPoll();
   }
   memset(outBuffer, 0, 8);
   outBuffer[0] = data;
   usbSetInterrupt(outBuffer, 8);
   return 1;
}

unsigned char buffer[32];
int __attribute__((noreturn)) main(void)
{
   uchar   i;
   DDRB |= (1 << PB1); //define DDRB before doing usb stuffs

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
        if (is_available())
          {
             int size = readchar(buffer);
             if (size != 0)
               {
                  writechar(buffer[0]);
                  writechar('\n');
               }
          }

        usbPoll();
     }
}
