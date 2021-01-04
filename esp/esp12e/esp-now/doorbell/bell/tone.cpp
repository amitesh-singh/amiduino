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
}
