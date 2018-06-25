#define FOSC F_CPU
#define BAUD_RATE BAUD

#define MYUBRR FOSC/16/BAUD_RATE-1

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void USART_send( unsigned char data);

void USART_Init(unsigned int ubrr)
{
   UBRR0H = (unsigned char)(ubrr>>8);
   UBRR0L = (unsigned char)ubrr;
   /* Enable UART receiver and transmitter */

   UCSR0B = (1 << TXEN0) | (1<<RXEN0) | (1<<RXCIE0);
   UCSR0C |= (1<<UCSZ00 | (1 << UCSZ01) | (1 << USBS0));
}

ISR(USART_RX_vect)
{
   while(!(UCSR0A&(1<<RXC0))){};
   unsigned char received = UDR0;
   USART_send(received);
}

void USART_send( unsigned char data)
{
   while(!(UCSR0A & (1<<UDRE0)));
   UDR0 = data;
}

int main(void)
{
   USART_Init(MYUBRR);
   DDRC |= (1 << PC5);
   sei();
   USART_send('I');
   while(1)
     {
        PORTC ^= (1 << PC5);
        _delay_ms(500);
     }
   return 0;
}
