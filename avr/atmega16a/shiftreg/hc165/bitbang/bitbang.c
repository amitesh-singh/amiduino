#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#if defined(__AVR_ATmega16A__)
#warning ("This code is compiled for __AVR_ATmega16A__");
#endif

#define S_DDR DDRB
#define S_PORT PORTB
#define S_PIN PINB

//For 74HC165 Serial-In/Parallel Out shift register
enum BitOrder
{
   LSB,
   MSB
};

void shift_init(uint8_t lpin, uint8_t dpin, uint8_t cpin)
{
   //Latch and clock pin are set OUTPUT
   S_DDR |= (1 << lpin) | (1 << cpin);
   // data pin is set INPUT
   S_DDR &= ~(1 << dpin);

   //Set both latch and clock port low
   S_PORT &= ~(1 << lpin);
   S_PORT &= ~(1 << cpin);
}

//RULE: for reading parallel-in and serial out - hc165n shift register
//
// make latch pin to go high
// make latch pin to go low
// read each bit of data to data pin based on bitOrder
//     after each bit read, Pulse clock pin 
//
//supporting only 8 bit right now
void shift_in(uint8_t lpin, uint8_t dpin, uint8_t cpin, enum BitOrder bitorder, uint8_t *val)
{
   uint8_t i = 0;

   S_PORT |= (1 << lpin);
   S_PORT &= ~(1 << lpin);
   uint8_t current_byte = 0;
   for (; i < 8; ++i)
     {
        //set clock pin high
        S_PORT |= (1 << cpin);
        if (bitorder == LSB)
          {
             current_byte |= (S_PIN & (1 << dpin)) << i;
          }
        else
          {
             current_byte |= (S_PIN & (1 << dpin)) << (7 - i);
          }
        //set clock pin low
        S_PORT &= ~(1 <<cpin);
     }

   *val = current_byte;
}

int main()
{
      //ss           / MOSI      // CLK 
   int lpin = PB4, dpin = PB5, cpin = PB7;
   shift_init(lpin, dpin, cpin);
   uint8_t value;

   while (1)
     {
        //read the datapin in every 10 ms
        shift_in(lpin, dpin, cpin, MSB, &value);
        _delay_ms(10);
     }

   return 0;
}

