#ifndef _I2C_SOFT_MASTER_H
#define _I2C_SOFT_MASTER_H

#include <avr/io.h>

void i2c_init();
uint8_t start(uint8_t rawAddr);
void stop();

#endif