#ifndef __MORSE_H
#define __MORSE_H

#include "Arduino.h" //Note: This is not required for main sketch since it is automatically added by compiler.
                     // This is important for a library to include it.
                     // More details: https://www.arduino.cc/en/Hacking/LibraryTutorial

class Morse
{
public:
  Morse(int pin);
  void dot();
  void dash();
private:
  int _pin;
};

#endif
