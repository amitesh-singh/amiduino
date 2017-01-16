#include <avr/io.h>
#include <util/delay.h>

#define DATAPIN PB0
#define CLOCKPIN PB2
#define LOADPIN PB4 //latch pin 

static void
_hc165_init()
{
  DDRB |= (1 << CLOCKPIN) | (1 << LOADPIN);
  DDRB &= ~(1 << DATAPIN);

  PORTB &= ~((1 << CLOCKPIN) | (1 << LOADPIN));
}

static void
_hc165_read(unsigned char *data)
{
  PORTB &= ~(1 << LOADPIN);
  _delay_us(5);
  PORTB |= (1 << LOADPIN);

  uint8_t currentByte = 0;

  for (uint8_t i = 0; i < 8; ++i)
  {
    currentByte |= (((PINB) & (1 << DATAPIN)) >> DATAPIN) << (7 - i);

    PORTB |= (1 << CLOCKPIN);
    _delay_us(5);
    PORTB &= ~(1 << CLOCKPIN);
  }
  
  *data = currentByte;
}

int main()
{
  _hc165_init();

  DDRB |= (1 << PB3);
  //just blink led
  PORTB |= (1 << PB3);
  _delay_ms(1000);
  PORTB &= ~(1 << PB3);
  _delay_ms(1000);
  
  uint8_t data = 0;

  while (1)
  {
    _hc165_read(&data);
    //this reads the value at D0
    if (data == 1) //|| data == 2 || data == 3)
    {
      PORTB &= ~(1 << PB3);
    }
    else
       PORTB |= (1 << PB3);
    //_delay_ms(10);     
  }

  return 0;
}