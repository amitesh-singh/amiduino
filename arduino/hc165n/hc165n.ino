/*
               ________
   (9)  latch  |      | Vcc
   (13) SCK    |      | CE - chip enable ---> GND
               |      |
               |      |
               |      |
               |      |
               |      | DS <-- From next
      GND      |      | Q7 --> MISO (12)
               --------
*/ 

#include <SPI.h>

const byte LATCH = 9;

void setup ()
{
  SPI.begin ();
  Serial.begin (115200);
  Serial.println ("Begin switch test.");
  pinMode (LATCH, OUTPUT);
  digitalWrite (LATCH, HIGH);
}  // end of setup

byte optionSwitch;
byte oldOptionSwitch; // previous state

void loop ()
{
  digitalWrite (LATCH, LOW);    // pulse the parallel load latch
  digitalWrite (LATCH, HIGH);
  optionSwitch = SPI.transfer (0);
  Serial.print("val=");
  Serial.println(optionSwitch);
  
  byte mask = 1;
  for (int i = 1; i <= 8; i++)
    {
    if ((optionSwitch & mask) != (oldOptionSwitch & mask))
      {
      Serial.print ("Switch ");
      Serial.print (i);
      Serial.print (" now ");
      Serial.println ((optionSwitch & mask) ? "closed" : "open");
      }  // end of bit has changed
    mask <<= 1;  
    }  // end of for each bit
  
  oldOptionSwitch = optionSwitch;
  delay (10);   // debounce
}  // end of loop