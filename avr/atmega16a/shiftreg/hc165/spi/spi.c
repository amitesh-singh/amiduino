#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#if defined(__AVR_ATmega16A__)
#warning ("This code is compiled for __AVR_ATmega16A__");
#endif

#define S_DDR DDRB
#define S_PORT PORTB
#define S_PIN PINB

//For 74HC165 Parallel-In/serial Out shift register

void shift_init(uint8_t lpin, uint8_t dpin, uint8_t cpin)
{
   S_DDR |= (1 << lpin) | (1 << cpin);
   S_DDR &= ~(1 << dpin);

   S_PORT &= (1 << lpin) | (1 << cpin);
}

void spi_init()
{
   SPCR = (1 << SPE) | (1 << MSTR); //Start SPI as Master
}

uint8_t spi_send(uint8_t data)
{
   SPDR = data; //shift in some data
   while (!(SPSR & (1 << SPIF))); // wait for SPI process to finish

   return SPDR; //retrun the recieved data
}

int main()
{
   // SS         MISO     // SCK
   int lpin = PB4, dpin = PB6, cpin = PB7;
   shift_init(lpin, dpin, cpin);
   uint8_t ret;
   spi_init();

   while (1)
     {
        S_PORT |= (1 << lpin);
        S_PORT &= ~(1 << lpin);
        ret = spi_send(1);
        _delay_ms(200);
     }

   return 0;
}

