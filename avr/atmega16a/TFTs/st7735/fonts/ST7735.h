#ifndef __ST7735_H_
#define __ST7735_H_

#include "fonts.h"

uint8_t cs = 4;
uint8_t rst = 2;
uint8_t dc = 3;

#define DDR  DDRB
#define PORT PORTB

#define CS_LOW   PORT &= ~(1 << cs)
#define CS_HIGH  PORT |= (1 << cs)

#define DC_LOW   PORT &= ~(1 << dc)
#define DC_HIGH  PORT |= (1 << dc)

#define RST_LOW  PORT &= ~(1 << rst)
#define RST_HIGH PORT |=  (1 << rst)

#define ST7735_COLOR(red, green, blue)   ((unsigned int)( (( blue >> 3 ) << 11 ) | (( green >> 2 ) << 5  ) |  ( red  >> 3 )))

//some default colors
 #define BLACK                       ST7735_COLOR(0x00, 0x00, 0x00)
 #define WHITE                       ST7735_COLOR(0xFF, 0xFF, 0xFF)
 #define RED                         ST7735_COLOR(0xFF, 0x00, 0x00)
 #define GREEN                       ST7735_COLOR(0x00, 0xFF, 0x00)
 #define BLUE                        ST7735_COLOR(0x00, 0x00, 0xFF)
 #define YELLOW                      ST7735_COLOR(0xFF, 0xFF, 0x00)
 #define MAGENTA                     ST7735_COLOR(0xFF, 0x00, 0xFF)
 #define CYAN                        ST7735_COLOR(0x00, 0xFF, 0xFF)
 #define GRAY                        ST7735_COLOR(0x80, 0x80, 0x40)
 #define SILVER                      ST7735_COLOR(0xA0, 0xA0, 0x80)
 #define GOLD                        ST7735_COLOR(0xA0, 0xA0, 0x40)

// LCD controller commands

#define ST7735_NOP         (0x0)
#define ST7735_SWRESET     (0x01)
#define ST7735_SLPIN       (0x10)
#define ST7735_SLPOUT      (0x11)
#define ST7735_PTLON       (0x12)
#define ST7735_NORON       (0x13)
#define ST7735_INVOFF      (0x20)
#define ST7735_INVON       (0x21)
#define ST7735_GAMSET      (0x26)
#define ST7735_DISPOFF     (0x28)
#define ST7735_DISPON      (0x29)
#define ST7735_CASET       (0x2A)
#define ST7735_RASET       (0x2B)
#define ST7735_RAMWR       (0x2C)
#define ST7735_RAMRD       (0x2E)
#define ST7735_PTLAR       (0x30)
#define ST7735_TEOFF       (0x34)
#define ST7735_PEON        (0x35)
#define ST7735_MADCTL      (0x36)
#define ST7735_IDMOFF      (0x38)
#define ST7735_IDMON       (0x39)
#define ST7735_COLMOD      (0x3A)

#define ST7735_FRMCTR1     (0xB1)
#define ST7735_INVCTR      (0xB4)
#define ST7735_DISSET5     (0xB6)
#define ST7735_PWCTR1      (0xC0)
#define ST7735_PWCTR2      (0xC1)
#define ST7735_PWCTR3      (0xC2)
#define ST7735_VMCTR1      (0xC5)
#define ST7735_PWCTR6      (0xFC)
#define ST7735_GMCTRP1     (0xE0)
#define ST7735_GMCTRN1     (0xE1)

#endif