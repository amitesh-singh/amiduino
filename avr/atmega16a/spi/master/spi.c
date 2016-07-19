#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#if defined(__AVR_ATmega16A__)
#warning ("This code is compiled for __AVR_ATmega16A__");
#endif

#define S_DDR DDRB
#define S_PORT PORTB

//For 74HC595 Serial-In/Parallel Out shift register

// lpin - latchpin SS, dpin - datapin MOSI, cpin - clock pin SCK
void shift_init(uint8_t lpin, uint8_t dpin, uint8_t cpin)
{
   S_DDR |= (1 << lpin) | (1 << dpin) | (1 << cpin);
}

void spi_init()
{
   SPCR = (1<<SPE) | (1<<MSTR); //Start SPI as Master
}

void spi_end()
{
   SPCR = 0;
}

uint8_t spi_send(uint8_t data)
{
   SPDR = data; //shift in some data
   while (!(SPSR & (1 << SPIF))); // wait for SPI process to finish

   return SPDR;
}

int main()
{
           // SS         MOSI     // SCK
   int lpin = PB4, dpin = PB5, cpin = PB7;
   shift_init(lpin, dpin, cpin);
    unsigned int binary_counter = 0;
   spi_init();

   while (1)
     {
        //set Q1 high
        S_PORT &= ~(1 << lpin);
        spi_send(0b11000011);
        spi_send(binary_counter>>8);
        spi_send(binary_counter);
        S_PORT |= (1 << lpin);
        S_PORT &= ~(1 << lpin);
        binary_counter++;
        _delay_ms(200);

     }

   spi_end();

   return 0;
}

