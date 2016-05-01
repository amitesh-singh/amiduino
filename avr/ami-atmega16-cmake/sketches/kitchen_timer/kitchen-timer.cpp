/*

Copyright (C) 2015 Amitesh Singh <singh [dot] amitesh [at] gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>

*/
#include "Seg7Led.h"

#define SPEAKERPIN 10

byte bytes[] = {2, 3, 4, 5, 6, 7, 8};
Seg7Led led(bytes);
byte val = HIGH;
unsigned long startTime = 0;
byte isAlarm = 0;

// the setup routine runs once when you press reset:
void setup()
{
#ifdef DEBUG
   Serial.begin(9600);
#endif
   pinMode(9, INPUT);
   pinMode(SPEAKERPIN, OUTPUT);

   led.set(0);
   startTime = 0;

   tone(SPEAKERPIN, 400, 200);
   delay(200);
   noTone(10);

   digitalWrite(9, HIGH);
}

byte count = 0;

void loop()
{
   val = digitalRead(9);

   if (val == LOW)
     {
        if (count == 0 && isAlarm == 1)
          {
             noTone(SPEAKERPIN);
             isAlarm = 0;
             delay(1000);
#ifdef DEBUG
             Serial.println("Setting buzzer off and not increasing counting");
#endif
             //digitalWrite(9, HIGH);
             //count = 0;
             return;
          }
        ++count;
        if (count == 10)
          count = 0;
        led.set(count);
        delay(900);

        if (count > 0)
          {
#ifdef DEBUG
             Serial.println("Start the timer");
#endif
             startTime = millis();

             isAlarm = 0;
          }
     }
   else if (isAlarm == 0)
     {
        unsigned long timeLimit = ((unsigned long)(count)) * 60 * 1000;
        if (count > 0 && (millis() - startTime) >= timeLimit)
          {
             //timer expires
#ifdef DEBUG
             Serial.println("Timer expires");
#endif
             // reset the led to 0
             // make sound on buzzer in next cycle :)
             led.set(0);

             isAlarm = 1;
             // set count = 0
             count = 0;
             startTime = 0;
          }
     }

   if (isAlarm)
     {
        tone(SPEAKERPIN, 400, 200);
        delay(200);
        noTone(SPEAKERPIN);
     }
}
