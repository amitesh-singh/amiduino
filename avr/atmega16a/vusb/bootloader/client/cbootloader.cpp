/*
  cbootloader.cpp - part of flashtool for AVRUSBBoot, an USB bootloader for Atmel AVR controllers

  Thomas Fischl <tfischl@gmx.de>

  Creation Date..: 2006-03-18
  Last change....: 2006-06-25

  Parts are taken from the PowerSwitch project by Objective Development Software GmbH
*/

#include "cbootloader.h"

static int  usbGetStringAscii(usb_dev_handle *dev, int index, int langid, char *buf, int buflen)
{
char    buffer[256];
int     rval, i;

    if((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + index, langid, buffer, sizeof(buffer), 1000)) < 0)
        return rval;
    if(buffer[1] != USB_DT_STRING)
        return 0;
    if((unsigned char)buffer[0] < rval)
        rval = (unsigned char)buffer[0];
    rval /= 2;
    /* lossy conversion to ISO Latin1 */
    for(i=1;i<rval;i++){
        if(i > buflen)  /* destination buffer overflow */
            break;
        buf[i-1] = buffer[2 * i];
        if(buffer[2 * i + 1] != 0)  /* outside of ISO Latin1 range */
            buf[i-1] = '?';
    }
    buf[i-1] = 0;
    return i-1;
}


/* This project uses the free shared default VID/PID. If you want to see an
 * example device lookup where an individually reserved PID is used, see our
 * RemoteSensor reference implementation.
 */
static usb_dev_handle   *findDevice(void)
{
struct usb_bus      *bus;
struct usb_device   *dev;
usb_dev_handle      *handle = 0;

    usb_find_busses();
    usb_find_devices();
    for(bus=usb_busses; bus; bus=bus->next){
        for(dev=bus->devices; dev; dev=dev->next){
            if(dev->descriptor.idVendor == USBDEV_SHARED_VENDOR && dev->descriptor.idProduct == USBDEV_SHARED_PRODUCT){
                char    string[256];
                int     len;
                handle = usb_open(dev); /* we need to open the device in order to query strings */
                if(!handle){
                    fprintf(stderr, "Warning: cannot open USB device: %s\n", usb_strerror());
                    continue;
                }
                len = usbGetStringAscii(handle, dev->descriptor.iManufacturer, 0x0409, string, sizeof(string));
                if(len < 0){
                    fprintf(stderr, "warning: cannot query manufacturer for device: %s\n", usb_strerror());
                    goto skipDevice;
                }
                if(strcmp(string, "www.fischl.de") != 0)
                    goto skipDevice;
                len = usbGetStringAscii(handle, dev->descriptor.iProduct, 0x0409, string, sizeof(string));
                if(len < 0){
                    fprintf(stderr, "warning: cannot query product for device: %s\n", usb_strerror());
                    goto skipDevice;
                }
		//  fprintf(stderr, "seen product ->%s<-\n", string); 
                if(strcmp(string, "AVRUSBBoot") == 0)
                    break;
skipDevice:
                usb_close(handle);
                handle = NULL;
            }
        }
        if(handle)
            break;
    }
    if(!handle)
        fprintf(stderr, "Could not find USB device www.fischl.de/AVRUSBBoot\n");
    return handle;
}



CBootloader::CBootloader() {
  usb_init();
  if((usbhandle = findDevice()) == NULL){
    fprintf(stderr, "Could not find USB device \"AVRUSBBoot\" with vid=0x%x pid=0x%x\n", USBDEV_SHARED_VENDOR, USBDEV_SHARED_PRODUCT);
    exit(1);
  }
}

CBootloader::~CBootloader() {
  usb_close(usbhandle);
}

unsigned int CBootloader::getPagesize() {
  char       buffer[8];
  int                 nBytes;

  nBytes = usb_control_msg(usbhandle, 
			   USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, 
			   3, 0, 0, 
			   buffer, sizeof(buffer), 
			   5000);

  if (nBytes != 2) {
    fprintf(stderr, "Error: wrong response size in getPageSize: %d !\n", nBytes);
    exit(1);
  }

  return (buffer[0] << 8) | buffer[1];
}

void CBootloader::startApplication() {
  char       buffer[8];
  int                 nBytes;

  nBytes = usb_control_msg(usbhandle, 
			   USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, 
			   1, 0, 0, 
			   buffer, sizeof(buffer), 
			   5000);

  if (nBytes != 0) {
//    fprintf(stderr, "Error: wrong response size in startApplication: %d !\n", nBytes);
    fprintf(stderr, "Sent /startApplication/, bad response because AVR left bootloader: %d!\n", nBytes);
    fprintf(stderr, " - Don't care, Application should run fine..\n");
    exit(1);
  }
}


void CBootloader::writePage(CPage* page) {

  int nBytes;

  nBytes = usb_control_msg(usbhandle, 
			   USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, 
			   2, page->getPageaddress(), 0, 
			   (char*) page->getData(), page->getPagesize(), 
			   5000);

  if (nBytes != page->getPagesize()) {
    fprintf(stderr, "Error: wrong byte count in writePage: %d !\n", nBytes);
    exit(1);
  }
}

