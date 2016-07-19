#include <avr/io.h>
#include <util/delay.h>
// In this example, connected MOSI to MISO, the led attached at PB0 should blink  3 times

#if defined(__AVR_ATmega16A__)
#warning ("This code is compiled for __AVR_ATmega16A__");
#endif

#define S_DDR DDRB
#define S_PORT PORTB

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
   spi_init();

   while (1)
     {
        //lower the lpin
        S_PORT &= ~(1 << lpin);
        int8_t led_blink = spi_send(5);
        S_PORT |= (1 << lpin); //high the pin
        S_PORT &= ~(1 << lpin); // lower the pin

        //5 times blink
        for (; led_blink > 0; --led_blink)
          {
             PORTB |= (1 << PB0);
             _delay_ms(250);
             PORTB &= ~(1 << PB0);
             _delay_ms(250);
          }
        _delay_ms(10000); // delay for 10 seconds
     }

   spi_end();

   return 0;
}

