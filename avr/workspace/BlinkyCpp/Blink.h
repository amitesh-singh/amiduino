/*
 * Blink.h
 *
 *  Created on: Nov 3, 2016
 *      Author: ami
 */

#ifndef BLINK_H_
#define BLINK_H_
#include <avr/io.h>
#include <util/delay.h>

class Blink {
public:
	Blink();
	void run();
};

#endif /* BLINK_H_ */
