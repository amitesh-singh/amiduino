// Minimal V-USB joystick example
/*

connection:


 RESET/PB6 |    | PB3      (D-)
 Up    PB5 |    | PB2/INT0 (D+)
 Down  PB4 |    | PB1  - Left
       GND |    | PB0  - Right
 */

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "usbdrv.h"

// Y, X, buttons (up to 8)
static uint8_t report [3]; // current
static uint8_t report_out [3]; // last sent over USB

static void joystick_init()
{
   // Configure as inputs with pull-ups
   DDRB  &= ~((1 << PB0) | (1 << PB1) | (1 << PB4) | (1 << PB5));
   PORTB |=  (1 << PB0) | (1 << PB1) | (1 << PB4) | (1 << PB5);
}

static void joystick_read()
{
   report [0] = 0;
   report [1] = 0;
   report [2] = 0;

   // Y
   if (bit_is_clear(PINB, PB5)) report [0] = -127; // Up
   if (bit_is_clear(PINB, PB4)) report [0] = +127; // Down

   // X
   if (bit_is_clear(PINB, PB1)) report [1] = -127; // Left 
   if (bit_is_clear(PINB, PB0)) report [1] = +127; // Right

   /*
   // Buttons
   if ( ! (PIND & 0x01) ) report [2] |= 0x01;
   if ( ! (PIND & 0x02) ) report [2] |= 0x02;
   // ...
    */
}

// X/Y joystick w/ 8-bit readings (-127 to +127), 8 digital buttons
PROGMEM const char usbHidReportDescriptor [USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
     0x05, 0x01,     // USAGE_PAGE (Generic Desktop)
     0x09, 0x05,     // USAGE (Game Pad)
     0xa1, 0x01,     // COLLECTION (Application)
     0x09, 0x01,     //   USAGE (Pointer)
     0xa1, 0x00,     //   COLLECTION (Physical)
     0x09, 0x30,     //     USAGE (X)
     0x09, 0x31,     //     USAGE (Y)
     0x15, 0x81,     //   LOGICAL_MINIMUM (-127)
     0x25, 0x7f,     //   LOGICAL_MAXIMUM (127)
     0x75, 0x08,     //   REPORT_SIZE (8)
     0x95, 0x02,     //   REPORT_COUNT (2)
     0x81, 0x02,     //   INPUT (Data,Var,Abs)
     0xc0,           // END_COLLECTION
     0x05, 0x09,     // USAGE_PAGE (Button)
     0x19, 0x01,     //   USAGE_MINIMUM (Button 1)
     0x29, 0x08,     //   USAGE_MAXIMUM (Button 8)
     0x15, 0x00,     //   LOGICAL_MINIMUM (0)
     0x25, 0x01,     //   LOGICAL_MAXIMUM (1)
     0x75, 0x01,     // REPORT_SIZE (1)
     0x95, 0x08,     // REPORT_COUNT (8)
     0x81, 0x02,     // INPUT (Data,Var,Abs)
     0xc0            // END_COLLECTION
};

uint8_t usbFunctionSetup( uint8_t data [8] )
{
   usbRequest_t const* rq = (usbRequest_t const*) data;

   if ( (rq->bmRequestType & USBRQ_TYPE_MASK) != USBRQ_TYPE_CLASS )
     return 0;

   switch ( rq->bRequest )
     {
      case USBRQ_HID_GET_REPORT: // HID joystick only has to handle this
         usbMsgPtr = (void *)&report_out;
         return sizeof report_out;

         //case USBRQ_HID_SET_REPORT: // LEDs on joystick?

      default:
         return 0;
     }
}

static void toggle_led( void )
{
   // No port left for toggle led
   //DDRC  |= 1;
   //PORTC ^= 1;
}

int main( void )
{
   usbInit();
   sei();

   joystick_init();

   while (1)
     {
        usbPoll();

        // Don't bother reading joy if previous changes haven't gone out yet.
        // Forces delay after changes which serves to debounce controller as well.
        if ( usbInterruptIsReady() )
          {
             joystick_read();

             // Don't send update unless joystick changed
             if ( memcmp( report_out, report, sizeof report ) )
               {
                  memcpy( report_out, report, sizeof report );
                  usbSetInterrupt( report_out, sizeof report_out );
                  toggle_led();
               }
          }
     }

   return 0;
}
