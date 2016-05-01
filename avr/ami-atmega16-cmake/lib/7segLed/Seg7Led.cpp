#include "Seg7Led.h"

byte Seg7Led::convert(byte value)
{
    switch(value)
    {
      case 1:
       return 6;
      case 2:
      return 91;
      case 3:
      return 79;
      case 4:
      return 102;
      case 5:
      return 109;
      case 6:
      return 125;
      case 7:
      return 7;
      case 8:
      return 127;
      case 9:
      return 111;
      case 0:
      return 63;
      default:
      return -1;
    }
  }

  void Seg7Led::reset()
  {
    for (byte i = 0; i < 7; ++i)
      digitalWrite(ledIndex[i], ishighv ? HIGH : LOW);
  }

  Seg7Led::Seg7Led(byte bytes[])
  {
    ledIndex = bytes;
    for (byte i = 0; i < 7; ++i)
    {
      pinMode(bytes[i], OUTPUT);
    }
    ishighv = 0;
  }

  void Seg7Led::set(byte value)
  {
    reset(); // reset all pins to low
    value = convert(value);
    for (byte i = 0; i <= 7; ++i)
    {
      if ((value >> (i)) & 1)
      {
        digitalWrite(ledIndex[i], ishighv ? LOW : HIGH);
      }
    }
  }

  void Seg7Led::setHighV(byte o)
  {
    if (o == ishighv) return;
    ishighv = o;
  }

  bool Seg7Led::isHighV()
  {
    return !!ishighv;
  }

void Seg7Led::setDot(byte on, byte pin)
{
  digitalWrite(pin, on);  
}
