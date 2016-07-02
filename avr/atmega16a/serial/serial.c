#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

//At 1 MHz, BAUD set to 2400 worked. 9600 did not work.
// http://wormfood.net/avrbaudcalc.php
// conceptually 4800 also should work
// fuse the atmega16a to 8mhz internal oscillator
// now i can use 9600UL and it works.
// fuse settings:-U lfuse:w:0xe4:m -U hfuse:w:0x99:m
#define BAUD 9600UL                           // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)    // set baudrate value for UBRR

#ifndef F_CPU
#define F_CPU 8000000UL                    // set the CPU clock
#endif

// function to initialize UART
void uart_init (void)
{
   UBRRH = (BAUDRATE>>8);
   UBRRL = BAUDRATE;                         //set baud rate
   UCSRB |= (1<<TXEN) | (1<<RXEN);             //enable receiver and transmitter
   UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);// 8bit data format
}

// function to send data - NOT REQUIRED FOR THIS PROGRAM IMPLEMENTATION
void uart_transmit (unsigned char data)
{
   //both lines are same.
   //while (!( UCSRA & (1<<UDRE)));            // wait while register is free
   loop_until_bit_is_set(UCSRA, UDRE);
   UDR = data;                             // load data in the register
}

void uart_print(const char *USART_String)
{
   uint8_t c;
   while ((c=*USART_String++))
     {
        uart_transmit(c);
     }
}//USART_Print

// function to receive data
unsigned char uart_recieve (void)
{
   //both statements are same
   //while(!(UCSRA & (1<<RXC)));           // wait while data is being received
   loop_until_bit_is_set(UCSRA, RXC);
   return UDR;                             // return 8-bit data
}

// main function: entry point of program
int main (void)
{
   uart_init();
   DDRB |= (1 << PB0);

   while (1)
     {
        PORTB |= (1 << PB0);
        uart_print("LED ON\r\n");
        _delay_ms(1000);
        PORTB = 0x0;
        uart_print("LED OFF\r\n");
        _delay_ms(1000);
     }

   return 0;
}
