
#include <Arduino.h>

#define led 2

// This example blinks the LED (led) in every 1 second
static const unsigned long CPU_MHZ = 80000000; //80Mhz -> 80*10^6 = 1 second
//volatile uint8_t toggle;

void inline timer0_ISR(void)
{
   //toggle = (toggle == 1) ? 0: 1;
   //digitalWrite(led, toggle);
   digitalWrite(led, !digitalRead(led)); 
   // if you want timer0 to be called just once, don't call timer0_write the below stuff
   // Set-up the next interrupt cycle
   timer0_write(ESP.getCycleCount() + CPU_MHZ);
}

void setup()
{
   pinMode(led, OUTPUT);
   noInterrupts();
   timer0_isr_init();
   timer0_attachInterrupt(timer0_ISR);

   timer0_write(ESP.getCycleCount() + CPU_MHZ);

   interrupts();
}

void loop()
{
   delay(1000);
}
