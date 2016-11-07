/*
 * ST7735.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: ami
 */

#include "ST7735.h"
#include <avr/io.h>
#include "fonts.h"

#include <util/delay.h>

//can it be configurable?

#define DDR  DDRB
#define PORT PORTB

#define CS_LOW   PORT &= ~(1 << _cs)
#define CS_HIGH  PORT |= (1 << _cs)

#define DC_LOW   PORT &= ~(1 << _dc)
#define DC_HIGH  PORT |= (1 << _dc)

#define RST_LOW  PORT &= ~(1 << _rst)
#define RST_HIGH PORT |=  (1 << _rst)


ST7735::ST7735(uint8_t cs, uint8_t rst, uint8_t dc, bool landscape): _cs(cs),
   _rst(rst), _dc(dc),
   _isLandscape(landscape)
{
   _colormode = 0b110; // color666 mode
}

ST7735::~ST7735()
{
   _spiEnd();
}

void ST7735::_delay_1ms(uint32_t ms)
{
   uint32_t i = 0;
   for (; i < ms; ++i)
     {
        _delay_ms(1);
     }
}

void ST7735::_spiInit()
{
	// SPE - enable SPI, MSTR  - act as a master spi device.
   SPCR = (1 << SPE) | (1 << MSTR);
   // f_osc/2 speed = 8 MHz
   SPSR |= (1 << SPI2X);

   // This is hardcoded for mega16
   DDRB |= (1 << PB7) | (1 << PB5);
}

void ST7735::_spiEnd()
{
   SPCR = 0;
}

uint8_t ST7735::_spiSend(uint8_t data)
{
   SPDR = data; //shift in some data
   while (!(SPSR & (1 << SPIF))); // wait for SPI process to finish

   return SPDR;
}

void ST7735::_writeCmd(uint8_t cmd)
{
   DC_LOW;
   CS_LOW;
   _spiSend(cmd);
   DC_HIGH;
}

void ST7735::_writeData(uint8_t data)
{
   _spiSend(data); //is it inline?
}

void ST7735::_writeWord(uint16_t word)
{
   _spiSend(word >> 8); // MSB
   _spiSend(word & 0xFF); // LSB
}

void ST7735::_write3Byte(const uint8_t *data)
{
	uint8_t i = 0;
	for (; i < 3 ; ++i)
	{
		_spiSend(pgm_read_byte(data + i));
	}
}

void ST7735::_setAddrWindow(uint8_t x0, uint8_t y0,
                            uint8_t x1, uint8_t y1)
{
   _writeCmd(ST7735_CASET);
   _writeWord(x0);
   _writeWord(x1);

   _writeCmd(ST7735_RASET);
   _writeWord(y0);
   _writeWord(y1);
}

void ST7735::init()
{
   // Set control pins to output
   DDRB |= _BV(_cs) | _BV(_rst) | _BV(_dc);

   // init SPI
   CS_HIGH;
   _spiInit();
   _spiSend(0);

   CS_LOW;
   DC_HIGH;

   // Reset display by hardware
   if (_rst) // ignore if _rst is zero.
     {
        RST_HIGH;
        _delay_1ms(100); //TODO: use delay()
        RST_LOW;
        _delay_1ms(100);
        RST_HIGH;
        _delay_1ms(100);
     }

   CS_HIGH;

   _initDisplay();
}

void ST7735::drawPixel(uint8_t x, uint8_t y, const uint8_t *color)
{
   _setAddrWindow(x, y, x + 1, y + 1);
   _writeCmd(ST7735_RAMWR);
   _write3Byte(color);

   _writeCmd(ST7735_NOP);
   CS_HIGH;
}

void ST7735::fillRec(uint8_t x, uint8_t y, uint8_t w,
                     uint8_t h, const uint8_t *color)
{
   unsigned int i = 0;

   _setAddrWindow(x, y, x + w - 1, y + h -1);
   _writeCmd(ST7735_RAMWR);

   for (; i < (w * h); ++i)
     {
	   _write3Byte(color);
     }

   _writeCmd(ST7735_NOP);
   CS_HIGH;
}

void ST7735::fillScreen(const uint8_t *color)
{
   fillRec(0, 0, getWidth(), getHeight(), color);
}

void ST7735::drawHLine(uint8_t x0, uint8_t y,
                       uint8_t x1, const uint8_t *color)
{
   uint8_t pixels = 0;

   _setAddrWindow(x0, y, getWidth(), y);
   _writeCmd(ST7735_RAMWR);

   for (; pixels < (x1 - x0); ++pixels)
     {
	   _write3Byte(color);
     }

   _writeCmd(ST7735_NOP);
   CS_HIGH;
}

void ST7735::drawVLine(uint8_t x, uint8_t y0, uint8_t y1,
		const uint8_t *color)
{
   uint8_t pixels = 0;

   _setAddrWindow(x, y0, x, getHeight());
   _writeCmd(ST7735_RAMWR);  // write to RAM
   for (; pixels < y1 - y0 ; pixels++)
     {
	   _write3Byte(color);
     }

   _writeCmd(ST7735_NOP);
   CS_HIGH;

}

void ST7735::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
		const uint8_t *color)
{
   //TODO: there are bugs, fix it after visiting trignometry.
   uint16_t pixels;
   int8_t c;

   float m;
   //note:
   //SetAddrWindow(x0, y0, x1 - x0, y1 - y0);
   //WriteCmd(ST7735_RAMWR);

   m = (y1 - y0)/(x1 - x0);
   c = y0 - m * x0;

   for (pixels = x0; pixels <(x1 - x0); ++pixels)
     {
        uint8_t y = m * pixels + c;
        drawPixel(pixels, y, color);
     }

}

void ST7735::drawRect(uint8_t posX, uint8_t posY, uint8_t width,
                      uint8_t height, const uint8_t *color)
{
   drawHLine(posX, posY, posX + width - 1, color);
   drawHLine(posX, posY + height - 1, posX + width, color);

   drawVLine(posX, posY, posY + height - 1, color);
   drawVLine(posX + width - 1, posY, posY + height - 1, color);
}

void ST7735::drawCircle(uint8_t posX, uint8_t posY, uint8_t r,
		const uint8_t *color)
{
   int x = -r, y = 0, err = 2-2*r, e2;
   do
     {
        drawPixel(posX - x, posY + y, color);
        drawPixel(posX + x, posY + y, color);
        drawPixel(posX + x, posY - y, color);
        drawPixel(posX - x, posY - y, color);

        e2 = err;
        if (e2 <= y)
          {
             err += ++y*2+1;
             if (-x == y && e2 <= x) e2 = 0;
          }

        if (e2 > x) err += ++x*2+1;

     } while (x <= 0);
}

void ST7735::fillCircle(uint8_t posX, uint8_t posY, uint8_t r,
		const uint8_t *color)
{
   int x = -r, y = 0, err = 2-2*r, e2;
   do
     {
        drawVLine(posX - x, posY - y, posY - y + 2*y, color);
        drawVLine(posX + x, posY - y, posY - y + 2*y, color);

        e2 = err;
        if (e2 <= y)
          {
             err += ++y*2+1;
             if (-x == y && e2 <= x) e2 = 0;
          }

        if (e2 > x) err += ++x*2+1;

     } while (x <= 0);
}

uint8_t ST7735::drawChar(uint8_t charToWrite, uint8_t poX,
                         uint8_t poY, uint8_t size, const uint8_t *fgcolor)
{
   int i;
   charToWrite -= 32;

   unsigned char char_width=pgm_read_byte(&Levo_Font[charToWrite+6]);

   int font_data = pgm_read_byte(&Levo_Font[5])+6;
   int font_offset = 0;

   for(i=0; i < charToWrite; i++)
     font_offset=font_offset + pgm_read_byte(&Levo_Font[6+i]);

   for(i = 0; i < char_width; i++)
     {
        unsigned char temp = pgm_read_byte(&Levo_Font[font_data + font_offset + i]);
        for(unsigned char f = 0; f < 8; f++)
          {
             if((temp>>f) & 0x01)
               {
                  if(size==1)
                    {
                       drawPixel(poX+i*size, poY+f*size, fgcolor);
                    }
                  else
                    {
                       fillRec(poX+i*size, poY+f*size, size, size, fgcolor);
                    }
               }
          }
     }

   return char_width;

}

void ST7735::drawString(const char *string,
                        unsigned char poX, unsigned char poY,
                        unsigned char size, const uint8_t *fgcolor)
{
   unsigned char c,w;

   while ( (c = *string++) )
     {
        w = drawChar(c, poX, poY, size, fgcolor);
        if(poX < getWidth())
          {
             poX += w*size;                                     /* Move cursor right            */
          }
     }

}

void   ST7735::drawXBitmap(uint8_t x, uint8_t y, const uint8_t *bitmap,
                           uint16_t w, uint16_t h, const uint8_t *color)
{
   uint16_t i, j, byteWidth = (w + 7) / 8;
   uint8_t byte;

   for(j = 0; j < h; j++)
     {
        for(i = 0; i < w; i++ )
          {
             if(i & 7) byte >>= 1;
             else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
             if(byte & 0x01) drawPixel(x + i, y + j, color);
          }
     }

}

void ST7735::drawBitmap(uint8_t x, uint8_t y, const uint8_t *bitmap,
                        uint16_t w, uint16_t h, const uint8_t *color)
{
   uint16_t i, j, byteWidth = (w + 7) / 8;
   uint8_t byte;

   for(j = 0; j < h; j++)
     {
        for(i = 0; i < w; i++)
          {
             if(i & 7) byte <<= 1;
             else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
             if(byte & 0x80) drawPixel(x + i, y + j, color);
          }
     }
}

void ST7735::_initDisplay()
{
   //TODO: push them into PROGMEM and then iterate like adafruit does?

   _writeCmd(ST7735_SWRESET); // software reset
   _delay_1ms(150);
   CS_HIGH;

   _writeCmd(ST7735_SLPOUT);  // out of sleep mode
   CS_HIGH;
   _delay_1ms(120);

   _writeCmd(ST7735_COLMOD);  // set color mode
   // 011 --> 12 bits/pixel
   // 101 --> 16 bits/pixel
   // 110 --> 18 bits/pixel
   _writeData(_colormode);          // 18-bit color

   _writeCmd(ST7735_FRMCTR1); // frame rate control
   _writeData(0x00);          // fastest refresh
   _writeData(0x06);          // 6 lines front porch
   _writeData(0x03);          // 3 lines backporch

   _writeCmd(ST7735_MADCTL);  // memory access control (directions)
   //_writeData(0xC8);          // row address/col address, bottom to top refresh
   //    MY, MX, MV, ML,      RGB, MH, -, -
   //C8 = 1,  1,  0,  0,        1,  0, 0, 0
   //_writeData( _BV(3));
   //    1   0    1   0         1   0  0  0


   if (_isLandscape)
     _writeData( _BV(7)| _BV(5) | _BV(3));
   else
     _writeData( _BV(3));

   _writeCmd(ST7735_DISSET5); // display settings #5
   _writeData(0x15);          // 1 clock cycle nonoverlap, 2 cycle gate rise, 3 cycle oscil. equalize
   _writeData(0x02);          // fix on VTL

   /*
      _writeCmd(ST7735_INVCTR);  // display inversion control
      _writeData(0x0);           // line inversion
      ST7735_CS_HI;
    */


   _writeCmd(ST7735_PWCTR1);  // power control
   _writeData(0x02);          // GVDD = 4.7V
   _writeData(0x70);          // 1.0uA

   _writeCmd(ST7735_PWCTR2);  // power control
   _writeData(0x05);          // VGH = 14.7V, VGL = -7.35V
   _writeCmd(ST7735_PWCTR3);  // power control
   _writeData(0x01);          // Opamp current small
   _writeData(0x02);          // Boost frequency

   _writeCmd(ST7735_VMCTR1);  // power control
   _writeData(0x3C);          // VCOMH = 4V
   _writeData(0x38);          // VCOML = -1.1V

   _writeCmd(ST7735_PWCTR6);  // power control
   _writeData(0x11);
   _writeData(0x15);

   _writeCmd(ST7735_GMCTRP1);
   _writeData(0x09);
   _writeData(0x16);
   _writeData(0x09);
   _writeData(0x20);
   _writeData(0x21);
   _writeData(0x1B);
   _writeData(0x13);
   _writeData(0x19);
   _writeData(0x17);
   _writeData(0x15);
   _writeData(0x1E);
   _writeData(0x2B);
   _writeData(0x04);
   _writeData(0x05);
   _writeData(0x02);
   _writeData(0x0E);
   _writeCmd(ST7735_GMCTRN1);
   _writeData(0x0B);
   _writeData(0x14);
   _writeData(0x08);
   _writeData(0x1E);
   _writeData(0x22);
   _writeData(0x1D);
   _writeData(0x18);
   _writeData(0x1E);
   _writeData(0x1B);
   _writeData(0x1A);
   _writeData(0x24);
   _writeData(0x2B);
   _writeData(0x06);
   _writeData(0x06);
   _writeData(0x02);
   _writeData(0x0F);
   /*
      _writeCmd(ST7735_CASET);   // column addr set
      _writeData(0x00);
      _writeData(0x00);          // XSTART = 2
      _writeData(0x00);
      _writeData(0x81);          // XEND = 129

      _writeCmd(ST7735_RASET);   // row addr set
      _writeData(0x00);
      _writeData(0x00);          // XSTART = 1
      _writeData(0x00);
      _writeData(0x81);          // XEND = 160
    */
   _writeCmd(ST7735_NORON);   // normal display on
   CS_HIGH;

   //TODO: Write your own delay() like arduino sdk has. you need to use Timer0 for that
   _delay_1ms(120);

   _writeCmd(ST7735_DISPON);
   CS_HIGH;
   _delay_1ms(120);
}
