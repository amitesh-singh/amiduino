### installation & setup
- need sudo apt-get install libusb-1.0
- http://blog.borg.ch/?p=342

### board
- stm32f103c8t6
-  `st-info --probe`  
Found 1 stlink programmers  
 serial: 303030303030303030303031  
openocd: "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x31"  
  flash: 65536 (pagesize: 1024)  
   sram: 20480  
 chipid: 0x0410  
  descr: F1 Medium-density device  

### libopencm3 APIs guide
- Each GPIO has its own clock signal. and this is defined in 
<libopencm3/stm32/rcc.h>. The Port A and Port C are activated by
rcc_peripheral_clcok_enable(RCC_GPIOA);  
rcc_peripheral_clcok_enable(RCC_GPIOC);  
- timers : http://www.kaltpost.de/?page_id=412
- STM32 arduino - http://www.wifi4things.com/stm32f103c8t6-blue-pill-board-with-arduino-ide-on-linux/
 use stlink as programmer instead.
 