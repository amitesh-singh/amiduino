#include <Arduino.h>

static const unsigned long CPU_MHZ = 160e6; 

static void _timer1_cb()
{
  digitalWrite(D4, !digitalRead(D4));
}

void setup()
{
  Serial.begin(115200);
  Serial.println("timer1 setup");

  pinMode(D4, OUTPUT);

  //timer1 setup
  timer1_disable();
  timer1_attachInterrupt(_timer1_cb);
  timer1_isr_init();
  /*
  //timer dividers
#define TIM_DIV1 	0 //80MHz (80 ticks/us - 104857.588 us max)
#define TIM_DIV16	1 //5MHz (5 ticks/us - 1677721.4 us max)
#define TIM_DIV265	3 //312.5Khz (1 tick = 3.2us - 26843542.4 us max)
//timer int_types
#define TIM_EDGE	0
#define TIM_LEVEL	1
//timer reload values
#define TIM_SINGLE	0 //on interrupt routine you need to write a new value to start the timer again
#define TIM_LOOP	1 //on interrupt the counter will start with the same value again
  */
  //tim_div1 can not support this
  //timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
  //with tim_loop, no need to call that write again
  //160/16 = 10 mhz, seems like max it can support is 0.8s
  //timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  //160mhz/265 = 
  timer1_enable(TIM_DIV265, TIM_EDGE, TIM_LOOP);
  timer1_write(ESP.getCycleCount() + CPU_MHZ/265);
}

void loop()
{

}