#include <avr/io.h>
#include <util/delay.h>

extern void led_init();
extern void set_pb0_high();
extern void set_pb0_low();

int main()
{
  led_init();
  
  while (1)
  {
    set_pb0_high();
    _delay_ms(1000);
    set_pb0_low();
    _delay_ms(1000);
  }

  return 0;
}