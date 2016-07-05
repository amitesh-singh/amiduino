#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "fifo.h"

#define BAUDRATE 38400
#define BUFSIZE_IN  0x40

#define F_CPU 20000000

uint8_t inbuf[BUFSIZE_IN];
fifo_t infifo;

#define BUFSIZE_OUT 0x40
uint8_t outbuf[BUFSIZE_OUT];
fifo_t outfifo;

#define UART_BAUD_SELECT(baudRate,xtalCpu) ((xtalCpu)/((baudRate)*16l)-1)


void uart_init(void)
{
    uint8_t sreg = SREG;
    uint16_t ubrr = UART_BAUD_SELECT(BAUDRATE, F_CPU);
    UBRR0H = (uint8_t) (ubrr >> 8);
    UBRR0L = (uint8_t) (ubrr);
    cli();
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    do {
        uint8_t dummy;
        (void) (dummy = UDR0);
    } while (UCSR0A & (1 << RXC0));
    UCSR0A = (1 << RXC0) | (1 << TXC0);
    fifo_init(&infifo, inbuf, BUFSIZE_IN);
    fifo_init(&outfifo, outbuf, BUFSIZE_OUT);
    SREG = sreg;
}



int uart_putc(unsigned char c)
{
    while (!(UCSR0A & (1<<UDRE0)))  /* warten bis Senden moeglich */
    {
    }                             
 
    UDR0 = c;                      /* sende Zeichen */
    return 0;
}
 
 
/* puts ist unabhaengig vom Controllertyp */
void uart_puts (char *s)
{
    while (*s)
    {   /* so lange *s != '\0' also ungleich dem "String-Endezeichen" */
        uart_putc(*s);
        s++;
    }
}

