/*
 * colors.h
 *
 *  Created on: Nov 8, 2016
 *      Author: ami
 */

#ifndef COLORS_H_
#define COLORS_H_
#include <avr/pgmspace.h>

//TODO: Add more colors
										  //b    //g   //r
static const uint8_t PROGMEM BLACK[] =   {0x00, 0x00, 0x00};
static const uint8_t PROGMEM RED[] =     {0x00, 0x00, 0xFF};
static const uint8_t PROGMEM GREEN[] =   {0x00, 0xFF, 0x00};
static const uint8_t PROGMEM BLUE[] =    {0xFF, 0x00, 0x00};
static const uint8_t PROGMEM YELLOW[] =  {0xFF, 0xFF, 0xFF};
static const uint8_t PROGMEM MAGNETA[] = {0xFF, 0x00, 0xFF};
static const uint8_t PROGMEM CYAN[] =    {0xFF, 0xFF, 0x00};
static const uint8_t PROGMEM GRAY[] =    {0x40, 0x80, 0x80};
static const uint8_t PROGMEM GOLD_YELLOW[] = {0, 223, 255};
static const uint8_t PROGMEM GOLD_METAL[] = {55, 175, 212};



#endif /* COLORS_H_ */
