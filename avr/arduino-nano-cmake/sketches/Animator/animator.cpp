/*
 * animator.cpp
 *
 *  Created on: Dec 4, 2016
 *      Author: ami
 */

#include "animator.h"
#include <avr/interrupt.h>
#include "Arduino.h"
static uint8_t counter;
static animinfo animList[5];

volatile uint8_t interrupt_called = 0;

ISR(TIMER1_COMPA_vect)
{
   interrupt_called = 1;
   Serial.println("interrupt is called");
}

void animator::add(anim_cb cb, void *data)
{
   animList[counter]._anim = this;
   animList[counter]._anim_cb = cb;
   animList[counter]._data = data;

   counter++;
}

void animator::del(anim_cb, void *data)
{
   counter--;
}

coreloop::coreloop(uint8_t fps)
{
   //this is bit tricky
   //run the timer2
   interrupt_called = 0;
   cli();
   TCCR1A = 0;
   TCCR1B = 0;
   TIMSK1 = 0;
   //TCNT1 = 0;
   //clock_tick = 0;

   //TCCR1A |= (1 << COM1A1);
   TCNT1 = 0;
   TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);  // prescalar = 1024, CTC mode

   //1 second tick
   OCR1A = 15624; //1 second tick  = 1/(1/16000000/1024)) - 1
   TIMSK1 |= (1 << OCIE1A);

   sei();
}

void coreloop::loop()
{
   if (!interrupt_called) return;
   uint8_t i = 0;
   for (; i < counter; ++i)
     {
        animList[i]._anim_cb(animList[i]._data);
     }
   interrupt_called = 0;

}



