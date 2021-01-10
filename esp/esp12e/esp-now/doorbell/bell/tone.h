#ifndef _TONE_H
#define _TONE_H
#include <Ticker.h>

namespace sound
{
    class tone
    {
        public:
        int _pin;
        void play(int pin, int freq, int len);
        void stop();

        void play_imperial_march(int pin);
        void play_close_encounter(int pin);
        void play_ringer(int pin);
        void play_harry_potter(int);
        void play_tone_down(int);
        void play_tone_up(int);
    };
}

#endif