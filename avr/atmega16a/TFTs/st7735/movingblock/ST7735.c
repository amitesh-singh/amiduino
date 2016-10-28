#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ST7735.h"

static void
SpiInit()
{
   SPCR = (1 << SPE) | (1 << MSTR);

   DDRB |= (1 << PB7) | (1 << PB5);
}

static void
SpiEnd()
{
   SPCR = 0;
}

static uint8_t
SpiSend(uint8_t data)
{
   SPDR = data; //shift in some data
   while (!(SPSR & (1 << SPIF))); // wait for SPI process to finish

   return SPDR;
}

static void
WriteCmd(uint8_t command)
{
   DC_LOW;
   CS_LOW;
   SpiSend(command);
   DC_HIGH;
}

inline void
WriteData(uint8_t data)
{
   //SpiSend(data);
   //make the code inline for FAST spi
   SPDR = data; //shift in some data
   while (!(SPSR & (1 << SPIF))); // wait for SPI process to finish
}

static void
WriteWord(uint16_t data)
{
   SpiSend(data >> 8);
   SpiSend(data & 0xFF);
}

static void
SetAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
   WriteCmd(ST7735_CASET);
   WriteWord(x0);
   WriteWord(x1);

   WriteCmd(ST7735_RASET);
   WriteWord(y0);
   WriteWord(y1);
}

void InitDisplay(void)
{
   WriteCmd(ST7735_SWRESET); // software reset
   _delay_ms(150);
   CS_HIGH;

   WriteCmd(ST7735_SLPOUT);  // out of sleep mode
   CS_HIGH;
   _delay_ms(120);

   WriteCmd(ST7735_COLMOD);  // set color mode
   WriteData(0x05);          // 16-bit color

   WriteCmd(ST7735_FRMCTR1); // frame rate control
   WriteData(0x00);          // fastest refresh
   WriteData(0x06);          // 6 lines front porch
   WriteData(0x03);          // 3 lines backporch

   WriteCmd(ST7735_MADCTL);  // memory access control (directions)
   //WriteData(0xC8);          // row address/col address, bottom to top refresh
   //    MY, MX, MV, ML,      RGB, MH, -, -
   //C8 = 1,  1,  0,  0,        1,  0, 0, 0
   //WriteData( _BV(3));
   //    1   0    1   0         1   0  0  0


#ifdef LANDSCAPE
   WriteData( _BV(7)| _BV(5) | _BV(3));
#else
   WriteData( _BV(3));
#endif


   WriteCmd(ST7735_DISSET5); // display settings #5
   WriteData(0x15);          // 1 clock cycle nonoverlap, 2 cycle gate rise, 3 cycle oscil. equalize
   WriteData(0x02);          // fix on VTL

   /*
      WriteCmd(ST7735_INVCTR);  // display inversion control
      WriteData(0x0);           // line inversion
      ST7735_CS_HI;
    */


   WriteCmd(ST7735_PWCTR1);  // power control
   WriteData(0x02);          // GVDD = 4.7V
   WriteData(0x70);          // 1.0uA

   WriteCmd(ST7735_PWCTR2);  // power control
   WriteData(0x05);          // VGH = 14.7V, VGL = -7.35V
   WriteCmd(ST7735_PWCTR3);  // power control
   WriteData(0x01);          // Opamp current small
   WriteData(0x02);          // Boost frequency

   WriteCmd(ST7735_VMCTR1);  // power control
   WriteData(0x3C);          // VCOMH = 4V
   WriteData(0x38);          // VCOML = -1.1V

   WriteCmd(ST7735_PWCTR6);  // power control
   WriteData(0x11);
   WriteData(0x15);

   WriteCmd(ST7735_GMCTRP1);
   WriteData(0x09);
   WriteData(0x16);
   WriteData(0x09);
   WriteData(0x20);
   WriteData(0x21);
   WriteData(0x1B);
   WriteData(0x13);
   WriteData(0x19);
   WriteData(0x17);
   WriteData(0x15);
   WriteData(0x1E);
   WriteData(0x2B);
   WriteData(0x04);
   WriteData(0x05);
   WriteData(0x02);
   WriteData(0x0E);
   WriteCmd(ST7735_GMCTRN1);
   WriteData(0x0B);
   WriteData(0x14);
   WriteData(0x08);
   WriteData(0x1E);
   WriteData(0x22);
   WriteData(0x1D);
   WriteData(0x18);
   WriteData(0x1E);
   WriteData(0x1B);
   WriteData(0x1A);
   WriteData(0x24);
   WriteData(0x2B);
   WriteData(0x06);
   WriteData(0x06);
   WriteData(0x02);
   WriteData(0x0F);
   /*
      WriteCmd(ST7735_CASET);   // column addr set
      WriteData(0x00);
      WriteData(0x00);          // XSTART = 2
      WriteData(0x00);
      WriteData(0x81);          // XEND = 129

      WriteCmd(ST7735_RASET);   // row addr set
      WriteData(0x00);
      WriteData(0x00);          // XSTART = 1
      WriteData(0x00);
      WriteData(0x81);          // XEND = 160
    */
   WriteCmd(ST7735_NORON);   // normal display on
   CS_HIGH;
   _delay_ms(120);

   WriteCmd(ST7735_DISPON);
   CS_HIGH;
   _delay_ms(120);
}

void
DrawPixel(uint8_t x, uint8_t y, uint16_t color)
{
   SetAddrWindow(x, y, x + 1, y + 1);
   WriteCmd(ST7735_RAMWR);
   WriteWord(color);
}

void
FilledRec(unsigned char x, unsigned char y,
          unsigned char w, unsigned char h,
          uint16_t color)
{
   unsigned int i;

   SetAddrWindow(x, y, x + w - 1, y + h - 1);
   WriteCmd(ST7735_RAMWR);  // write to RAM

   for(i = 0;i < (w * h); i++)
     {
        WriteData(color >> 8);
        WriteData(color);
     }
   //ST7735_CS_HI;
}

void TftInit(void)
{
   // Set control pins to output
   DDRB |= _BV(cs) | _BV(rst) | _BV(dc);

   // init SPI
   CS_HIGH;
   SpiInit();
   SpiSend(0);

   CS_LOW;
   DC_HIGH;

   // Reset display by hardware
   RST_HIGH;
   _delay_ms(100);
   RST_LOW;
   _delay_ms(100);
   RST_HIGH;
   _delay_ms(100);
   CS_HIGH;


   InitDisplay();
}


int main()
{
   TftInit();

   _delay_ms(100);

   DrawPixel(10, 10, RED);

   //clean the display
   FilledRec(0, 0, 128, 160, WHITE);

   //FilledRec(12, 12, 20, 20, GREEN);
   uint8_t x = 0;
   while (1)
   {
      if (x > 128)
         x = 0;

      FilledRec(x, 12, 20, 20, GREEN);
      _delay_ms(20);
      FilledRec(x, 12, 20, 20, BLACK);
      ++x;
   }

   SpiEnd();

   return 0;
}
