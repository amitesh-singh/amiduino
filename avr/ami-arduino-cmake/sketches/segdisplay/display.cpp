
#include "Seg7Led.h"

byte bytes[] = {2, 3, 4, 5, 6, 7, 8}; // Arduino digital ports number
Seg7Led led(bytes);

void setup()
{
   // do nothing.
}

void loop()
{
   // Program to blink from 0 to 9.
   led.set(0); //display 0 on 7 segment LED
   delay(1000);
   led.set(9); // display 9 on 7 segment LED
   delay(1000);
}


