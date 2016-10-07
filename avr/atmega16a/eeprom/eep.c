#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h> //for EEMEM macro

//this stores the default PB0 value before going nuts
uint8_t EEMEM  port_initial_val = 1;  // this is at index 0

//call eeprom_read_word(&another_settings);
uint16_t EEMEM another_settings = 1024; // this is at index 1

// char buf[10];
//eeprom_read_block(buf, version_info, 10);
unsigned char EEMEM version_info[] = "Hello Ami";

int main(void)
{
   DDRB |= (1 << PB0);

   //event loop
   if (eeprom_read_byte(&port_initial_val))
     PORTB = (1 << PB0);
   else
     PORTB = 0;

   while (1)
     {
        PORTB |= (1 << PB0);
        _delay_ms(1000);

        PORTB &= ~(1 << PB0);
        _delay_ms(1000);
     }

   return 0;
}
