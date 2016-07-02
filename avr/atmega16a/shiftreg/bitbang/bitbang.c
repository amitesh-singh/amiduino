#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#if defined(__AVR_ATmega16A__)
#warning ("This code is compiled for __AVR_ATmega16A__");
#endif

#define S_DDR DDRB
#define S_PORT PORTB

//For 74HC595 Serial-In/Parallel Out shift register
// Experiment with 74HC165 when you get one: TODO:
enum BitOrder
{
   LSB,
   MSB
};

void shift_init(uint8_t lpin, uint8_t dpin, uint8_t cpin)
{
   S_DDR |= (1 << lpin) | (1 << dpin) | (1 << cpin);
}

//RULE:
//
// make latch pin to go LOW
// write each bit of data to data pin based on bitOrder
//     after each bit write, Pulse clock pin 
// make latch pin to go HIGH
//
//supporting only 8 bit right now
void shift_out(uint8_t lpin, uint8_t dpin, uint8_t cpin, enum BitOrder bitorder, uint8_t val)
{
   uint8_t i = 0;

   S_PORT = 0x00;
   S_PORT &= ~(1 << lpin);
   for (; i < 8; ++i)
     {
        if (bitorder == LSB)
          {
             if (val & 0b00000001)
               S_PORT |= (1 << dpin);
             else
               S_PORT &= ~(1 << dpin);

             val = val >> 1;
          }
        else
          {
             if (val & 0b10000000)
               S_PORT |= (1 << dpin);
             else
               S_PORT &= ~(1 << dpin);
             val = val << 1;
          }

        //pulse clock pin
        S_PORT &= ~(1 <<cpin);
        S_PORT |= (1 << cpin);
     }

   S_PORT |= (1 << lpin);
   S_PORT &= ~(1 << lpin);
}

int main()
{
   int lpin = PB4, dpin = PB5, cpin = PB7;
   shift_init(lpin, dpin, cpin);
   //issue with this was that, the led did remain on for very short period
   // of time. :( -- update: Issue was 
   // OE is not grounded :P. problem is solved. ;)
   // Keep ports high for 10 seconds
   shift_out(lpin, dpin, cpin, MSB, 0b00100111);
   _delay_ms(10000);

   while (1)
     {
        // blink 4 LEDs
        shift_out(lpin, dpin, cpin, MSB, 0b00100111);
         _delay_ms(200);
         shift_out(lpin, dpin, cpin, MSB, 0x00);
         _delay_ms(200);
        shift_out(lpin, dpin, cpin, MSB, 0b00100111);
         _delay_ms(100);
         shift_out(lpin, dpin, cpin, MSB, 0x00);
         _delay_ms(100);
     }

   return 0;
}

