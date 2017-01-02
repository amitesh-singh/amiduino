/*
 * spi.h
 *
 *  Created on: Jan 2, 2017
 *      Author: ami
 */

#ifndef SPI_H_
#define SPI_H_

#define MOSI PB3
#define MISO PB4
#define SCK PB5
#define SS PB3

void spi_init()
{
   DDRB |= (1 << MOSI) | (1 << SCK) | (1 << SS);
   DDRB &= ~(1 << MISO); //set MISO at input
   SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0); //f_ck/16 = 1Mhz
}

unsigned char spi_send(unsigned char data)
{
   SPDR = data; //rq->wValue.bytes[0];
   while (!(SPSR & (1 << SPIF)))
     {
        ;
     }

   return SPDR;
}

void spi_end()
{
   SPCR = 0;
}

#endif /* SPI_H_ */
