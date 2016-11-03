/*
 * Blink.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: ami
 */

#include "Blink.h"
#include <avr/io.h>
#include <util/delay.h>

Blink::Blink() {
	DDRB |= (1 << PB0);
}

void Blink::run() {
	PORTB |= (1 << PB0);
	_delay_ms(1000);
	PORTB &= ~(1 << PB0);
	_delay_ms(1000);
}

