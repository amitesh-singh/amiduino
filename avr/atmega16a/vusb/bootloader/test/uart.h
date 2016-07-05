#ifndef _UART_H_
#define _UART_H_

#define CR "\r\n"


#include <avr/io.h>
#include <avr/pgmspace.h>

extern void uart_init(void);
extern int uart_putc(unsigned char c);
extern void uart_puts(char *s);

static inline void uart_flush(void)
{
    while (UCSR0B & (1 << UDRIE0));
}


#endif                          /* _UART_H_ */
