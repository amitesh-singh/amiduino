#ifndef _I2C_SOFT_MASTER_H
#define _I2C_SOFT_MASTER_H

#include <avr/io.h>

//initialize the i2c peripherals
void i2c_begin();

//this starts the sequence 7 bit address + R/W(0/1), 1 - means master requesting data
// and 0 - master is sending data
uint8_t i2c_start(uint8_t rawAddr);

//this is Stop condition. SDA goes high after SCL
void i2c_stop();
//returns the ACK
uint8_t i2c_writeByte(uint8_t);
uint8_t i2c_readByte(uint8_t islast);

#endif