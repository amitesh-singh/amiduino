#include "tone.h"
#include "tones.h"
#include <ESP8266WiFi.h>

namespace sound
{
    void tone::play(int pin, int freq, int len)
    {
        pinMode(pin, OUTPUT);
        analogWriteFreq(freq);
        analogWrite(pin, 512);
    
        _pin = pin;

        delay(len);

        stop();
    }

    void tone::stop()
    {
         analogWrite(_pin, 0);
         pinMode(_pin, INPUT);
    }

    void tone::play_imperial_march(int pin)
    {
        _pin = pin;

        play(_pin, Note_A4, 500);
        play(_pin, Note_A4, 500);
        play(_pin, Note_A4, 500);
        play(_pin, Note_F4, 350);
        play(_pin, Note_C5, 150);
        play(_pin, Note_A4, 500);
        play(_pin, Note_F4, 350);
        play(_pin, Note_C5, 150);
        play(_pin, Note_A4, 650);
    }

    void tone::play_close_encounter(int pin)
    {
        _pin = pin;

        play(_pin, Note_G6, 300); // G6
        play(_pin, Note_A6, 300); // A6
        play(_pin, Note_F6, 300); // F6
        play(_pin, Note_F5, 300); // F5
        play(_pin, Note_C6, 300); // C6
    }

    void tone::play_ringer(int pin)
    {
       _pin = pin;

       play(_pin, Note_F6, 75);
       play(_pin, Note_F5, 75);
       play(_pin, Note_F6, 75);
       play(_pin, Note_F5, 75);
       play(_pin, Note_F6, 75);
       play(_pin, Note_F5, 75);
       play(_pin, Note_F6, 75);
       play(_pin, Note_F5, 75);
    }

void tone::play_harry_potter(int _pin)
 {

  play(_pin, Note_DS5, 300);
  play(_pin, Note_GS5, 450);
  play(_pin, Note_B5, 150);
  play(_pin, Note_A5, 300);
  play(_pin, Note_GS5, 600);
  play(_pin, Note_DS6, 300);
  play(_pin, Note_CS6, 750);
  play(_pin, Note_AS5, 750);

  play(_pin, Note_GS5, 450);
  play(_pin, Note_B5, 150);
  play(_pin, Note_A5, 300);

  //  play(Note_F5, 600);
  play(_pin, Note_G5, 600);

  play(_pin, Note_A5, 300);
  play(_pin, Note_DS5, 750);

} // End of Harry Potter

void tone::play_tone_down(int _pin) {
  play(_pin, Note_F6, 200); // F6
  play(_pin, Note_F5, 200); // F5
  play(_pin, Note_C6, 200); // C6
} // End of beep

void tone::play_tone_up(int _pin) {
  play(_pin, Note_C6, 200); // C6
  play(_pin, Note_F5, 200); // F5
  play(_pin, Note_F6, 200); // F6
} // End of beep

}
