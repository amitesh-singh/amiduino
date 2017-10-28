
#include <Arduino.h>

#define led 2

// This example blinks the LED (led) in every 1 second
static const unsigned long CPU_MHZ = 80000000; //80Mhz -> 80*10^6 = 1 second

void inline timer0_ISR(void)
{
   digitalWrite(led, !digitalRead(led)); 

   //there is no TIM_LOOP in timer0 so you would need to call this everytime. :/
   timer0_write(ESP.getCycleCount() + CPU_MHZ);   
}

void setup()
{
   pinMode(led, OUTPUT);
   noInterrupts();

   //start timer0, timer0 does not have below function like timer1 has
   //timer0_disable();
   timer0_attachInterrupt(timer0_ISR);
   timer0_isr_init();
   
   //timer0 does not have below function so we would need to call timer0_write in 
   //  timer0_ISR function.
   //timer0_enable(TIM_DIV265, TIM_EDGE, TIM_LOOP);
   timer0_write(ESP.getCycleCount() + CPU_MHZ);

   interrupts();
}

void loop()
{
   delay(1000);
}
