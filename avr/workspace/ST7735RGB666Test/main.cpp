/*
 * main.cpp
 *
 *  Created on: Nov 6, 2016
 *      Author: ami
 */

#include <avr/io.h>
#include "ST7735.h"
#include <avr/pgmspace.h>
#include "colors.h"

uint8_t cs = 4;
uint8_t rst = 2;
uint8_t dc = 3;

#define Mostly_Cloudy_width 12
#define Mostly_Cloudy_height 12
static const unsigned char Mostly_Cloudy_bits[] PROGMEM = {
   0x00, 0x00, 0x10, 0x00, 0x82, 0x00, 0x38, 0x00, 0x7c, 0x00, 0x05, 0x00,
   0xf0, 0x00, 0xf8, 0x03, 0xfc, 0x07, 0xfc, 0x07, 0xf0, 0x01, 0x00, 0x00 };


ST7735 st7735(cs, rst, dc);

int main()
{
	st7735.init();
	st7735.fillScreen(BLACK);
//	st7735.drawPixel(10, 10, RED);
//	st7735.fillRec(45, 45, 40, 30, BLUE);
//
//	st7735.drawLine(0, 0, 20, 100, GREEN);
//	st7735.drawString("Ami\nGoogle", 60, 50, 1, RED);
//
//	st7735.drawXBitmap(67, 34, Mostly_Cloudy_bits, Mostly_Cloudy_width, Mostly_Cloudy_height,
//			GOLD);

	st7735.drawHLine(0, 50, 128, BLUE);
	st7735.drawHLine(0, 100, 128, BLUE);
	st7735.drawVLine(42, 0, 160, BLUE);
	st7735.drawVLine(85, 0, 160, BLUE);

	st7735.drawCircle(50, 34, 20, RED);

	while (1)
	{
		//animate example in main loop.
		// i think the same can be done on timer too.
		//animate text?
		st7735.drawString("animate me", 10, 90, 2, GREEN);
		st7735._delay_1ms(1000);
		st7735.fillRec(10, 90, 90, 20, BLACK);
		st7735._delay_1ms(1000);
	}
	return 0;
}


