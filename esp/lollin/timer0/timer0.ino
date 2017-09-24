#include <Arduino.h>
//This is a LOLLIN board which has D4 as led on board 
volatile uint8_t toggle;
//cpu speed == 1 s
// 1s = 160mhz (if esp12e is running at 160Mhz) you can run it at two speeds 80 or 160mhz, specify it in board type
//1000ms = 160e6
// 100 = 16e6

static const long cpu_mhz = 160e6;
void inline timer0_ISR(void)
{
   toggle = (toggle == 1) ? 0: 1;
   digitalWrite(D4, toggle);

   // if you want timer0 to be called just once, don't call timer0_write the below stuff
   // Set-up the next interrupt cycle
   // call it after 80mhz clock cycles which is equal to 1s
   timer0_write(ESP.getCycleCount() + cpu_mhz/10);
}

void setup()
{
   pinMode(D4, OUTPUT);

   noInterrupts();
   //timer0_disable();
   timer0_isr_init();
   timer0_attachInterrupt(timer0_ISR);
   //this will call the interrupt after 1000 clock cycles
   //timer0_write(ESP.getCycleCount() + 1000);
   timer0_write(ESP.getCycleCount() + cpu_mhz/10);

   interrupts();
}

void loop()
{
   delay(1000);
}
