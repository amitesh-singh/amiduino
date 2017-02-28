// External interrupt INT0 example - arduino uno rev 3 (Atmega328p)

#include <avr/sleep.h>


const byte BUTTON = 2;
const byte LED = 13;

void wakeup()
{
  Serial.println("Interrupt is called");
  if (digitalRead(BUTTON) == HIGH)
  {
    Serial.println("LED digital pin 13 is set low");
    digitalWrite(LED, LOW);
  }
  else
  {
    Serial.println("LED digital pin 13 is set high");
    digitalWrite(LED, HIGH);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
 // pinMode(BUTTON, INPUT); not needed since INPUT is the default state of pins
  digitalWrite(BUTTON, OUTPUT); // enable internal pullup resister, BUTTON is HIGH now

  // This will call inerrupt INT0 which is attached to PD2 aka digital pin 2
  // For calling int1 on uno (PD3) (Atmega328p), attachInterrupt(1, wakeup2, CHANGE);
  attachInterrupt(0, wakeup, CHANGE);  // can be LOW, HIGH, or CHANGE
}

void loop()
{
  // do nothing.
}
