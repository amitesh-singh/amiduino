#include <avr/io.h>

extern void set_pb0_high();
extern void loop();

int main()
{
  set_pb0_high();
    
  loop();

  return 0;
}