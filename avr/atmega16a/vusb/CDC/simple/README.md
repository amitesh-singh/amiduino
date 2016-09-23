### notes about cdc class - HID

1. following changes are required to do in usbconfig.h

`#define USB_CFG_DEVICE_CLASS        2 //CDC class`  
`#define USB_CFG_DEVICE_SUBCLASS     0`  
`#define USB_CFG_INTERFACE_CLASS     2`  
`#define USB_CFG_INTERFACE_SUBCLASS  2`  
`#define USB_CFG_INTERFACE_PROTOCOL  1`  

Add your custom report descriptor - check main.c

kernel will recognize it as following as virtual COM port
`
[ +13.254725] usb 2-1.8.4: new low-speed USB device number 47 using ehci-pci
[  +0.098533] usb 2-1.8.4: New USB device found, idVendor=16c0, idProduct=03e8
[  +0.000005] usb 2-1.8.4: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[  +0.000003] usb 2-1.8.4: Product: Mega16a
[  +0.000002] usb 2-1.8.4: Manufacturer: ami.com
[  +0.034442] cdc_acm 2-1.8.4:1.0: skipping garbage
[  +0.000030] usbcore: registered new interface driver cdc_acm
[  +0.000002] cdc_acm: USB Abstract Control Model driver for USB modems and ISDN adapters
`



