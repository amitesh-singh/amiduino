
#include <Arduino.h>

#define led 2

static const unsigned long CPU_MHZ = 80e6; //80Mhz -> 80*10^6 = 1 second

void inline timer1_ISR(void)
{
   digitalWrite(led, !digitalRead(led));
   Serial.println("Blinks.");
}

void setup() 
{
   Serial.begin(115200);

   pinMode(led, OUTPUT);
   digitalWrite(led, HIGH);
   
   //noInterrupts();

   //disable timer1 before configuring
   timer1_disable();
   timer1_attachInterrupt(timer1_ISR);
   timer1_isr_init();
   //No need to add ESP.getCycleCount() this results into weird results
   timer1_write(/*ESP.getCycleCount() +*/ CPU_MHZ/16);
   
   timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP); //other values are TIM_SINGLE

   // enable interrupts
   //interrupts();
}

void loop()
{
   delay(1000);
}
