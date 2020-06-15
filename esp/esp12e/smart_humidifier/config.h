#ifndef _HUMIDIFIER_CONFIG_H
#define _HUMIDIFIER_CONFIG_H

//Add your wifi ssid here
static const char ssid[] = "POOJA";

//Add your wifi password here
static const char password[] = "poojasingh";

#define MYESP12E_BOARD

//comment it in final code
#define DEBUG 

#ifdef LOLLIN_BOARD
// Pin Config
static const uint8_t led = D4; //inbuilt led pin
static const uint8_t pinDHT11 = D0;
static const uint8_t pinFan = D1;
static const uint8_t pinHeater = D2;
#endif

#ifdef MYESP12E_BOARD
static const uint8_t led = 2; //inbuilt led pin
static const uint8_t pinDHT11 = 12;
static const uint8_t pinFan = 14;
static const uint8_t pinHeater = 16;
#endif
//current consumption: esp12e(170mA) + Fan(170mA) + ultrasonic fogger(24V, 600mA)
#endif
