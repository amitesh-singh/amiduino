// Demo sketch to read from a 74HC165 input shift register
// Author: Nick Gammon
// Date:   23 March 2013

// Pin connections for Uno and similar:

// Chip pin 1 (/PL)  goes to LATCH (D9)  (or any other pin by changing LATCH below)
// Chip pin 2 (CP)   goes to SCK   (D13)
// Chip pin 9 (Q7)   goes to MISO  (D12)

// Pin connections for Mega2560:

// Chip pin 1 (/PL)  goes to LATCH (D9)  (or any other pin by changing LATCH below)
// Chip pin 2 (CP)   goes to SCK   (D52)
// Chip pin 9 (Q7)   goes to MISO  (D50)


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

   Serial.println("optionSwitch:");
   Serial.println(optionSwitch);
   //delay(200);
   
   byte mask = 1;
   for (int i = 1; i <= 8; i++)
     {
        if ((optionSwitch & mask) != (oldOptionSwitch & mask))
          {
             //Serial.print ("Switch ");
             //Serial.print (i);
             //Serial.print (" now ");
             //Serial.println ((optionSwitch & mask) ? "closed" : "open");
          }  // end of bit has changed
        mask <<= 1;  
     }  // end of for each bit

   oldOptionSwitch = optionSwitch;
   delay (100);   // debounce
}  // end of loop

