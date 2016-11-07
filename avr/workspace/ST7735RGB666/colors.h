/*
 * colors.h
 *
 *  Created on: Nov 8, 2016
 *      Author: ami
 */

#ifndef COLORS_H_
#define COLORS_H_
#include <avr/pgmspace.h>

// #define BLACK                       ST7735_COLOR(0x00, 0x00, 0x00)
// #define WHITE                       ST7735_COLOR(0xFF, 0xFF, 0xFF)
// #define RED                         ST7735_COLOR(0xFF, 0x00, 0x00)
// #define GREEN                       ST7735_COLOR(0x00, 0xFF, 0x00)
// #define BLUE                        ST7735_COLOR(0x00, 0x00, 0xFF)
// #define YELLOW                      ST7735_COLOR(0xFF, 0xFF, 0x00)
// #define MAGENTA                     ST7735_COLOR(0xFF, 0x00, 0xFF)
// #define CYAN                        ST7735_COLOR(0x00, 0xFF, 0xFF)
// #define GRAY                        ST7735_COLOR(0x80, 0x80, 0x40)
// #define SILVER                      ST7735_COLOR(0xA0, 0xA0, 0x80)
// #define GOLD                        ST7735_COLOR(0xA0, 0xA0, 0x40)

										  //r    g      b
static const uint8_t PROGMEM BLACK[] =   {0x00, 0x00, 0x00};
static const uint8_t PROGMEM RED[] =     {0xFF, 0x00, 0x00};
static const uint8_t PROGMEM GREEN[] =   {0x00, 0xFF, 0x00};
static const uint8_t PROGMEM BLUE[] =    {0x00, 0x00, 0xFF};
static const uint8_t PROGMEM YELLOW[] =  {0xFF, 0xFF, 0x00};
static const uint8_t PROGMEM MAGNETA[] = {0xFF, 0x00, 0xFF};
static const uint8_t PROGMEM CYAN[] =    {0x00, 0xFF, 0xFF};
static const uint8_t PROGMEM GRAY[] =    {0x80, 0x80, 0x40};


#endif /* COLORS_H_ */
