/*Copyright (C) 2015 Amitesh Singh <singh [dot] amitesh [at] gmail.com>

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

#ifndef __SEG7_LED_H
#define __SEG7_LED_H

#include <Arduino.h>

class Seg7Led
{
  byte *ledIndex;
  byte ishighv;
  byte convert(byte value);
public:
  void setHighV(byte o);
  void reset();
  Seg7Led(byte bytes[]);
  void set(byte value);
  bool isHighV();
  void setDot(byte on, byte pin);
};

#endif
