// Written by Amitesh Singh 2016

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>
/*

          ATmega16A
   	 
  	                 +---\/---+
         LED (D 0) PB0  1|        |40  PA0 (A 0 / D24)
             (D 1) PB1  2|        |39  PA1 (A 1 / D25)
        INT2 (D 2) PB2  3|        |38  PA2 (A 2 / D26)
         PWM (D 3) PB3  4|        |37  PA3 (A 3 / D27)
         /SS (D 4) PB4  5|        |36  PA4 (A 4 / D28)
        MOSI (D 5) PB5  6|        |35  PA5 (A 5 / D29)
       /MISO (D 6) PB6  7|        |34  PA6 (A 6 / D30)
        /SCK (D 7) PB7  8|        |33  PA7 (A 7 / D31)
                   RST  9|        |32  AREF
                   VCC 10|        |31  GND 
                   GND 11|        |30  AVCC
                 XTAL2 12|        |29  PC7 (D 23)
                 XTAL1 13|        |28  PC6 (D 22)
        RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
        TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
 (RX1)/INT0 (D 10) PD2 16|        |25  PC3 (D 19) TMS
 (TX1)/INT1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
        PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
        PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
            (D 14) PD6 20|        |21  PD7 (D 15) PWM
                         +--------+

****NO PCINTs, SoftwareSerial won't compile
*/

#define STANDARD_PINOUT
#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + 24 : -1)
#define digitalPinToInterrupt(p)  ((p) == 2 ? 2 : ((p) == 10 ? 0 : ((p) == 11 ? 1 : NOT_AN_INTERRUPT)))

#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 12 || (p) == 13 || (p) == 15)

static const uint8_t LED = 0;
#define LED_BUILTIN 0

static const uint8_t SS   = 4;
static const uint8_t MOSI = 5;
static const uint8_t MISO = 6;
static const uint8_t SCK  = 7;

static const uint8_t SDA = 17;
static const uint8_t SCL = 16;

static const uint8_t A0 = 24;
static const uint8_t A1 = 25;
static const uint8_t A2 = 26;
static const uint8_t A3 = 27;
static const uint8_t A4 = 28;
static const uint8_t A5 = 29;
static const uint8_t A6 = 30;
static const uint8_t A7 = 31;

#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
   NOT_A_PORT,
   (uint16_t) &DDRA,
   (uint16_t) &DDRB,
   (uint16_t) &DDRC,
   (uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
   NOT_A_PORT,
   (uint16_t) &PORTA,
   (uint16_t) &PORTB,
   (uint16_t) &PORTC,
   (uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
   NOT_A_PORT,
   (uint16_t) &PINA,
   (uint16_t) &PINB,
   (uint16_t) &PINC,
   (uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
   PB, /* D0 */
   PB,
   PB,
   PB,
   PB,
   PB,
   PB,
   PB,
   PD, /* D8 */
   PD,
   PD,
   PD,
   PD,
   PD,
   PD,
   PD,
   PC, /* D16 */
   PC,
   PC,
   PC,
   PC,
   PC,
   PC,
   PC,
   PA, /* D24 */
   PA,
   PA,
   PA,
   PA,
   PA,
   PA,
   PA  /* D31 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
   _BV(0), /* D0, port B */
   _BV(1),
   _BV(2),
   _BV(3),
   _BV(4),
   _BV(5),
   _BV(6),
   _BV(7),
   _BV(0), /* D8, port D */
   _BV(1),
   _BV(2),
   _BV(3),
   _BV(4),
   _BV(5),
   _BV(6),
   _BV(7),
   _BV(0), /* D16, port C */
   _BV(1),
   _BV(2),
   _BV(3),
   _BV(4),
   _BV(5),
   _BV(6),
   _BV(7),
   _BV(0), /* D24, port A */
   _BV(1),
   _BV(2),
   _BV(3),
   _BV(4),
   _BV(5),
   _BV(6),
   _BV(7)
};


const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
   NOT_ON_TIMER, 	/* D0  - PB0 */
   NOT_ON_TIMER, 	/* D1  - PB1 */
   NOT_ON_TIMER, 	/* D2  - PB2 */
   TIMER0A,       /* D3  - PB3 */
   NOT_ON_TIMER, 	/* D4  - PB4 */
   NOT_ON_TIMER, 	/* D5  - PB5 */
   NOT_ON_TIMER, 	/* D6  - PB6 */
   NOT_ON_TIMER,	/* D7  - PB7 */
   NOT_ON_TIMER, 	/* D8  - PD0 */
   NOT_ON_TIMER, 	/* D9  - PD1 */
   NOT_ON_TIMER, 	/* D10 - PD2 */
   NOT_ON_TIMER, 	/* D11 - PD3 */
   TIMER1B,     	/* D12 - PD4 */
   TIMER1A,     	/* D13 - PD5 */
   NOT_ON_TIMER,   /* D14 - PD6 */
   TIMER2,     	/* D15 - PD7 */
   NOT_ON_TIMER, 	/* D16 - PC0 */
   NOT_ON_TIMER,   /* D17 - PC1 */
   NOT_ON_TIMER,   /* D18 - PC2 */
   NOT_ON_TIMER,   /* D19 - PC3 */
   NOT_ON_TIMER,   /* D20 - PC4 */
   NOT_ON_TIMER,   /* D21 - PC5 */
   NOT_ON_TIMER,   /* D22 - PC6 */
   NOT_ON_TIMER,   /* D23 - PC7 */
   NOT_ON_TIMER,   /* D24 - PA0 */
   NOT_ON_TIMER,   /* D25 - PA1 */
   NOT_ON_TIMER,   /* D26 - PA2 */
   NOT_ON_TIMER,   /* D27 - PA3 */
   NOT_ON_TIMER,   /* D28 - PA4 */
   NOT_ON_TIMER,   /* D29 - PA5 */
   NOT_ON_TIMER,   /* D30 - PA6 */
   NOT_ON_TIMER    /* D31 - PA7 */
};

#endif // ARDUINO_MAIN

#endif // Pins_Arduino_h
