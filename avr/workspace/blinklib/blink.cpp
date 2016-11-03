/*
 * blink.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: ami
 */

#include "blink.h"
#include <avr/io.h>
#include <util/delay.h>

blink::blink()
{
	DDRB |= (1 << PB0);
}

blink::~blink()
{
	// TODO Auto-generated destructor stub
}

void blink::run()
{
	PORTB |= (1 << PB0);
	_delay_ms(1000);
	PORTB &= ~(1 << PB0);
	_delay_ms(1000);
}

