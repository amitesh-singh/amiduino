#include <avr/io.h>

extern void led_init();
extern void loop();

int main()
{
  led_init();
    
  loop();

  return 0;
}