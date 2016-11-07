/*
 * ST7735.h
 *
 *  Created on: Nov 3, 2016
 *      Author: ami
 */

#ifndef ST7735_H_
#define ST7735_H_

//using RGB565
//#define ST7735_COLOR(red, green, blue)   ((unsigned int)( (( blue >> 3 ) << 11 ) | (( green >> 2 ) << 5  ) |  ( red  >> 3 )))
// using RGB666
//#define ST7735_COLOR(red, green, blue)      (uint32_t)((red << 16) | (green << 8) | (blue))

//TODO: Add more colors..
//some default colors
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


#include <stdint.h>

class ST7735
{
	uint8_t 		_cs;
	uint8_t 		_rst;
	uint8_t 		_dc;

	// 011 --> 12 bits/pixel
    // 101 --> 16 bits/pixel
	// 110 --> 18 bits/pixel
	uint8_t 		_colormode;
	bool 			_isLandscape;


protected:
	/* SPI functions */
	void 			_spiInit();
	void 		 	_spiEnd();
	inline uint8_t 	_spiSend(uint8_t);

	/* LCD related functions */
	void 			_writeCmd(uint8_t);
	void 			_writeData(uint8_t);
	void 			_writeWord(uint16_t);
	void			_write3Byte(const uint8_t *);

	void 			_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

	//TODO: optimize it
	void 			_initDisplay();


public:
	void _delay_1ms(uint32_t ms);


	ST7735(uint8_t cs, uint8_t rst, uint8_t dc, bool landscape = false);
	~ST7735();

	void 		init();
	void 		drawPixel(uint8_t x, uint8_t y, const uint8_t *color);
	void 		fillRec(uint8_t x, uint8_t y, uint8_t w, uint8_t h,
			            const uint8_t *color);
	void 		fillScreen(const uint8_t *color);
	void 		drawHLine(uint8_t x0, uint8_t y, uint8_t x1, const uint8_t *color);
	void 		drawVLine(uint8_t x, uint8_t y0, uint8_t y1, const uint8_t *color);
	void 		drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
						 const uint8_t *color);
	void 		drawRect(uint8_t posX, uint8_t posY, uint8_t width,
						 uint8_t height, const uint8_t *color);
	void 		drawCircle(uint8_t posX, uint8_t posY, uint8_t r, const uint8_t *color);
	void 		fillCircle(uint8_t posX, uint8_t posY, uint8_t r, const uint8_t *color);
	void 		drawTriangle(); //TODO
	uint8_t 	drawChar(uint8_t charToWrite, uint8_t poX, uint8_t poY,
						 uint8_t size, const uint8_t *fgcolor);
	void 		drawString(const char *string, unsigned char poX, unsigned char poY,
            			   unsigned char size, const uint8_t *fgcolor);
	void 		drawXBitmap(uint8_t x, uint8_t y, const uint8_t *bitmap,
							uint16_t w, uint16_t h, const uint8_t *color);

	void 		drawBitmap(uint8_t x, uint8_t y, const uint8_t *bitmap,
			    		   uint16_t w, uint16_t h, const uint8_t *color);


	uint8_t 	getWidth()
	{
		if (_isLandscape)
			return 160;
		else
			return 128;
	}

	uint8_t 	getHeight()
	{
		if (_isLandscape)
			return 128;
		else
			return 160;
	}
};

#endif /* ST7735_H_ */
