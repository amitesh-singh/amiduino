#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

// in ms
const static uint16_t BLINK_LEDS_TIMEOUT = 100;

#define LEFT_SWITCH     PB0
#define RIGHT_SWITCH    PB1
#define LEFT_LED        PB3
#define RIGHT_LED       PB4

static volatile int8_t key_pressed = 0;
//1 PB0 is pressed
//2 PB1 is pressed 
// 0 none is pressed

static void my_delay(uint16_t ms)
{
  uint16_t i = 0;
  for (; i < ms; ++i)
  {
    _delay_ms(1);
  }
}

static void blinkLed(uint8_t pin)
{
  //TODO: we could use timer0 to blink led. but thats okay
  PORTB |= (1 << pin);
  my_delay(BLINK_LEDS_TIMEOUT);
  PORTB &= ~(1 << pin);
  my_delay(BLINK_LEDS_TIMEOUT);
}

/*
const static uint8_t DEBOUNCE_TIMEOUT = 100;

static uint8_t debounce(uint8_t pin)
{
   if (bit_is_clear(PINB, pin))
     {
        _delay_ms(DEBOUNCE_TIMEOUT);
        if (bit_is_clear(PINB, pin))
          {
             return 1;
          }
     }

   return 0;
}
*/

// PB0 - PCINT0
// PB1 - PCINT1
ISR(PCINT0_vect)
{
   //PB0 is pressed, left
   if (bit_is_clear(PINB, LEFT_SWITCH))
     {
        key_pressed = 1;
     }
   else if (bit_is_clear(PINB, RIGHT_SWITCH)) // PB1 is pressed, right
     {
        key_pressed = 2;
     }
   else
     {
        key_pressed = 0; //switch off the leds
     }
}

void blinkLeds()
{
   PORTB |= (1 << LEFT_LED) | (1 << RIGHT_LED);
   my_delay(BLINK_LEDS_TIMEOUT);
   //_delay_ms(BLINK_LEDS_TIMEOUT);
   PORTB &= ~((1 << LEFT_LED) | (1 << RIGHT_LED));
   //_delay_ms(BLINK_LEDS_TIMEOUT);
   my_delay(BLINK_LEDS_TIMEOUT);
}

//blink both LEFT and right key for 4 times
void startMechanism()
{
   uint8_t i = 0;
   for (; i < 4; ++i)
     {
        blinkLeds();
     }
}

int main(void)
{
   //left        right  
   DDRB |= (1 << LEFT_LED) | (1 << RIGHT_LED);

   // enable PB0 and PB1  internal pullups
   DDRB &= ~(1 << LEFT_SWITCH); // Left key
   DDRB &= ~(1 << RIGHT_SWITCH); // Right key
   PORTB |= (1 << LEFT_SWITCH) | (1 << RIGHT_SWITCH);


   GIMSK |= (1 << PCIE); // enable pcint 
   PCMSK |= (1 << PCINT1) | (1 << PCINT0);

   sei(); //set global enable interrupts!

   startMechanism();

   while (1)
     {
        //do not sleep
        if (key_pressed != 0)
          {
            if (key_pressed == 1)
            {
              //blink left led
              blinkLed(LEFT_LED);
            }
            else if (key_pressed == 2)
            {
              // blink right leds
              blinkLed(RIGHT_LED);
            }
          }
        else
          {
             //sleep
             PORTB &= ~((1 << LEFT_LED) | (1 << RIGHT_LED));
             cli();
             set_sleep_mode(SLEEP_MODE_PWR_DOWN);
             sleep_enable();
             power_all_disable();
             sei();

             sleep_mode();  // <<-- sleep here

             sleep_disable();
             power_all_enable();
             //TODO: disable everything like ADC, UART, timer?
          }
     }

   return 0;
}
