#include <avr/io.h>
#include <util/delay.h>

extern void led_init();
extern void set_pb0_high();
extern void set_pb0_low();
extern void delay_250ms();

int main()
{
  led_init();
  
  while (1)
  {
    set_pb0_high();
    delay_250ms();
    set_pb0_low();
    delay_250ms();
  }

  return 0;
}
