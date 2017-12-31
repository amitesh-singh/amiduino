/*
 * An AVR library for communication on a Dallas 1-Wire bus.
 * 
 * Features (?)
 * ------------
 * 
 * * It uses a single GPIO pin (no UARTs).
 * * It does not use dynamic memory allocation. The only drawback is that you
 *   have to know the number of devices on the bus in advance.
 * * It is polled, not interrupt-driven. There are several sections of code
 *   that must run for a specific amount of time and have to disable 
 *   interrupts globally.
 * * Only the MATCH_ROM, SEARCH_ROM, and SKIP_ROM commands have been 
 *   implemented. At this point other commands would be trivial to add.
 *
 * Directions
 * ----------
 *
 * 1. Modify F_CPU, DALLAS_PORT, DALLAS_DDR, DALLAS_PORT_IN, DALLAS_PIN, and
 *    DALLAS_NUM_DEVICES to match your application.
 * 2. In your code, first run dallas_search_identifiers() to populate the 
 *    the list of identifiers with the devices on your bus.
 * 3. ???
 * 4. Profit!
 *
 * Cautions/Caveats
 * ----------------
 * 
 * The 1-Wire bus is *very* timing-dependent. If you are having issues and it's
 * not an electrical/connectivity one it is most likely a timing issue. The 
 * worst function in this regard is dallas_read(). The delays chosen there are a
 * compromise between having to wait for the bus to return to 5V after being 
 * pulled to ground (determined by the RC time constant) while also needing to
 * read the bus before the 15 usec time slot expires.
 *
 * Verify the timing with a logic analyzer or oscilloscope. Also check the
 * datasheet for your specific device to make sure that it is ok with the timing
 * values chosen in the code and modify, if necessary.
 *
 * I am not a 1-Wire expert by any means so this code is provided as-is.
 *
 * Enjoy!
 * ------
 *
 * April 30, 2010
 */
 
/******************************************************************************
 * Copyright © 2010, Mike Roddewig (mike@dietfig.org).
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published 
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <avr/io.h>

#include <stdint.h>

#include "dallas_one_wire.h"

#include <util/atomic.h>
#include <util/delay.h>

//////////////////////
// Global variables //
//////////////////////

DALLAS_IDENTIFIER_LIST_t identifier_list;

/////////////////////////////////////
// Identifier routine return codes //
/////////////////////////////////////

#define DALLAS_IDENTIFIER_NO_ERROR 0x00
#define DALLAS_IDENTIFIER_DONE 0x01
#define DALLAS_IDENTIFIER_SEARCH_ERROR 0x02

/////////////////////////////////
// Private function prototypes //
/////////////////////////////////

static uint8_t dallas_discover_identifier(DALLAS_IDENTIFIER_t *, DALLAS_IDENTIFIER_t *);

///////////////
// Functions //
///////////////

void dallas_write(uint8_t bit) {
	if (bit == 0x00) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			// Configure the pin as an output.
			DALLAS_DDR |= _BV(DALLAS_PIN);
		
			// Pull the bus low.
			DALLAS_PORT &= ~_BV(DALLAS_PIN);
		
			// Wait the required time.
			_delay_us(90);
		
			// Release the bus.
			DALLAS_PORT |= _BV(DALLAS_PIN);
			
			// Let the rest of the time slot expire.
			_delay_us(30);
		}
	}
	else {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			// Configure the pin as an output.
			DALLAS_DDR |= _BV(DALLAS_PIN);
		
			// Pull the bus low.
			DALLAS_PORT &= ~_BV(DALLAS_PIN);
		
			// Wait the required time.
			_delay_us(10);
		
			// Release the bus.
			DALLAS_PORT |= _BV(DALLAS_PIN);
			
			// Let the rest of the time slot expire.
			_delay_us(50);
		}
	}
}

uint8_t dallas_read(void) {
	uint8_t reply;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		// Configure the pin as an output.
		DALLAS_DDR |= _BV(DALLAS_PIN);
	
		// Pull the bus low.
		DALLAS_PORT &= ~_BV(DALLAS_PIN);
	
		// Wait the required time.
		_delay_us(2);
	
		// Configure as input.
		DALLAS_DDR &= ~_BV(DALLAS_PIN);
		
		// Wait for a bit.
		_delay_us(11);
		
		if ((DALLAS_PORT_IN & _BV(DALLAS_PIN)) == 0x00) {
			reply = 0x00;
		}
		else {
			reply = 0x01;
		}
		
		// Let the rest of the time slot expire.
		_delay_us(47);
	}
	
	return reply;
}

// Resets the bus and returns 0x01 if a slave indicates present, 0x00 otherwise.
uint8_t dallas_reset(void) {
	uint8_t reply;
	
	// Reset the slave_reply variable.
	reply = 0x00;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
	
		// Configure the pin as an output.
		DALLAS_DDR |= _BV(DALLAS_PIN);
	
		// Pull the bus low.
		DALLAS_PORT &= ~_BV(DALLAS_PIN);
	
		// Wait the required time.
		_delay_us(500); // 500 uS
	
		// Switch to an input, enable the pin change interrupt, and wait.
		DALLAS_DDR &= ~_BV(DALLAS_PIN);
		
		_delay_us(70);
		
		if ((DALLAS_PORT_IN & _BV(DALLAS_PIN)) == 0x00) {
			reply = 0x01;
		}
		
		_delay_us(420);
	}
	
	return reply;
}

void dallas_write_byte(uint8_t byte) {
	uint8_t position;
	
	for (position = 0x00; position < 0x08; position++) {
		dallas_write(byte & 0x01);
		
		byte = (byte >> 1);
	}
}

uint8_t dallas_read_byte(void) {
	uint8_t byte;
	uint8_t position;
	
	byte = 0x00;
	
	for (position = 0x00; position < 0x08; position++) {
		byte += (dallas_read() << position);
	}
	
	return byte;
}

// Uses the uC to power the bus.
void dallas_drive_bus(void) {
	// Configure the pin as an output.
	DALLAS_DDR |= _BV(DALLAS_PIN);
	
	// Set the bus high.
	DALLAS_PORT |= _BV(DALLAS_PIN);
}

void dallas_match_rom(DALLAS_IDENTIFIER_t * identifier) {
	uint8_t identifier_bit;
	uint8_t current_byte;
	uint8_t current_bit;
	
	dallas_reset();
	dallas_write_byte(MATCH_ROM_COMMAND);
	
	for (identifier_bit = 0x00; identifier_bit < DALLAS_NUM_IDENTIFIER_BITS; identifier_bit++) {
		current_byte = identifier_bit / 8;
		current_bit = identifier_bit - (current_byte * 8);
		
		dallas_write(identifier->identifier[current_byte] & _BV(current_bit));
	}
}

void dallas_skip_rom(void) {
	dallas_reset();
	dallas_write_byte(SKIP_ROM_COMMAND);
}

uint8_t dallas_search_identifiers(void) {
	uint8_t current_device;
	uint8_t return_code;
	
	for (current_device = 0x00; current_device < DALLAS_NUM_DEVICES; current_device++) {
		if (current_device == 0x00) {
			return_code = dallas_discover_identifier(&identifier_list.identifiers[current_device], 0x00);
		}
		else {
			return_code = dallas_discover_identifier(&identifier_list.identifiers[current_device], &identifier_list.identifiers[current_device-1]);
		}
		
		if (return_code == DALLAS_IDENTIFIER_DONE) {
			identifier_list.num_devices = current_device + 0x01;
			return 0x00;
		}
		else if (return_code == DALLAS_IDENTIFIER_SEARCH_ERROR) {
			return 0x01;
		}
	}
	
	return 0x02;
}

DALLAS_IDENTIFIER_LIST_t * get_identifier_list(void) {
	return &identifier_list;
}

static uint8_t dallas_discover_identifier(DALLAS_IDENTIFIER_t * current_identifier, DALLAS_IDENTIFIER_t * last_identifier) {
	uint8_t identifier_bit;
	uint8_t received_two_bits;
	uint8_t current_bit;
	uint8_t current_byte;
	uint8_t identifier_diverged;
	
	identifier_diverged = 0x00;
	identifier_bit = 0x00;
	
	dallas_reset();
	dallas_write_byte(SEARCH_ROM_COMMAND);
	
	for (identifier_bit = 0; identifier_bit < DALLAS_NUM_IDENTIFIER_BITS; identifier_bit++) {
		received_two_bits = (dallas_read() << 1);
		received_two_bits += dallas_read();
		
		current_byte = identifier_bit / 8;
		current_bit = identifier_bit - (current_byte * 8);
		
		if (received_two_bits == 0x02) {
			// All devices have a 1 at this position.
			
			current_identifier->identifier[current_byte] += (1 << current_bit);
			
			dallas_write(0x01);
		}
		else if (received_two_bits == 0x01) {
			// All devices have a 0 at this position.
			
			dallas_write(0x00);
		}
		else if (received_two_bits == 0x00) {
			if ((identifier_diverged == 0x00) && (last_identifier != 0x00)) {
				identifier_diverged = 0x01;
				
				if ((last_identifier->identifier[current_byte] & _BV(current_bit)) == 0x00) {
					// Then we choose 1.
					
					current_identifier->identifier[current_byte] += (1 << current_bit);
					
					dallas_write(0x01);
				}
				else {
					// Otherwise 0.
					
					dallas_write(0x00);
				}
			}
			else {
				// We'll go with 0.
				dallas_write(0x00);
			}
		}
		else {
			// Error!
			return DALLAS_IDENTIFIER_SEARCH_ERROR;
		}
	}
	
	if (identifier_diverged == 0x00) {
		return DALLAS_IDENTIFIER_DONE;
	}
	else {
		return DALLAS_IDENTIFIER_NO_ERROR;
	}
}

void dallas_write_buffer(uint8_t * buffer, uint8_t buffer_length) {
	uint8_t i;
	
	for (i = 0x00; i < buffer_length; i++) {
		dallas_write_byte(buffer[i]);
	}
}

void dallas_read_buffer(uint8_t * buffer, uint8_t buffer_length) {
	uint8_t i;
	
	for (i = 0x00; i < buffer_length; i++) {
		buffer[i] = dallas_read_byte();
	}
}