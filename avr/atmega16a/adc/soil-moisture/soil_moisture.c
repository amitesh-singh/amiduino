#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include <stdio.h>

//NOTE: comment the below line in production code
#define DEBUG

#define BAUD 9600UL                           // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)    // set baudrate value for UBRR

#define SOIL_MOISTURE_THRESHOLD 300

#ifdef DEBUG 
static void
uart_init (void)
{
   UBRRH = (BAUDRATE >> 8);
   UBRRL = BAUDRATE;                         //set baud rate
   UCSRB |= (1 << TXEN) | (1 << RXEN);             //enable receiver and transmitter
   UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);// 8bit data format
}

// function to send data - NOT REQUIRED FOR THIS PROGRAM IMPLEMENTATION
static void
uart_transmit (unsigned char data)
{
   //both lines are same.
   //while (!( UCSRA & (1<<UDRE)));            // wait while register is free
   loop_until_bit_is_set(UCSRA, UDRE);
   UDR = data;                             // load data in the register
}

static void
uart_print(const char *USART_String)
{
   uint8_t c;
   while ((c=*USART_String++))
     {
        uart_transmit(c);
     }
}//USART_Print

// function to receive data
unsigned char uart_recieve (void)
{
   //both statements are same
   //while(!(UCSRA & (1<<RXC)));           // wait while data is being received
   loop_until_bit_is_set(UCSRA, RXC);
   return UDR;                             // return 8-bit data
}
#endif

static void
adc_init()
{
   // ADC Multiplexer Selection Register
   ADMUX = (1 << REFS0);

   // ADCSRA - ADC control and status Register A
   // ADEN - ADC enable
   ADCSRA = (1 << ADEN) |
      (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
}

static uint16_t
adc_read(uint8_t ch)
{
   // make sure value remains between 0 and 7
   ch &= 0b00000111;

   // clears the bottom 3 bit before ORing
   ADMUX = (ADMUX & 0xF8) | ch;

   // Write 1 to ADSC
   ADCSRA |= (1 << ADSC);

   //Wait till ADSC becomes 0
   while (ADCSRA & (1 << ADSC));

   return ADC;
}

static int8_t is_motor_started = 0;

static void
motor_init()
{
   DDRB = (1 << PB0);
}

static void
motor_start()
{
   PORTB = (1 << PB0);
}

static void
motor_stop()
{
   PORTB = 0;
}

static unsigned int timer_overflow = 0;

/*ISR(TIMER1_OVF_vect)
{
   uart_print("timer 1 overflow happened\r\n");
}
*/
ISR(TIMER0_OVF_vect)
{
   ++timer_overflow;
}

static void
timer0_init()
{
   TCCR0 |= ((1 << CS00) | (1 << CS02));

   //start the timer
   TCNT0 = 0;
   timer_overflow = 0;
   TIMSK |= (1 << TOIE0);
}

static void
timer0_stop()
{
   TCCR0 = 0;
}

static void
timer1_init()
{
   TCCR1A = 0;
   TCNT1 = 0;
   TCCR1B = (1 << CS10) | (1 << CS12); // prescalar = 1024

   TIMSK |= (1 << TOIE1);
}

static void
timer1_stop()
{
   TCCR1B = 0;
   TIMSK = 0;
}

int main()
{
   uint16_t adc_result;

#ifdef DEBUG
   char buf[15];
   uart_init();
#endif
   adc_init();
   motor_init();
   timer1_init();
   sei();

   DDRB = (1 << PB0); // set it as output

   while (1)
     {
        if (is_motor_started == 0)
          {
             //TODO: don't read continously, may be sleep for long time and then read it?

             adc_result = adc_read(0); // read PA0 port

#ifdef DEBUG
             sprintf(buf, "val=%d\r\n", adc_result);
             uart_print(buf);
#endif
             //NOTE: 225 was for TAP water
             // 540 - 580 for a POT with OK moisture
             // LOWER means more moisture
             if (adc_result >= SOIL_MOISTURE_THRESHOLD
                 && adc_result < 1000)
               {
                  motor_start();
                  is_motor_started = 1;
                  //stop the sleep stuff
                  timer1_stop();
                  timer0_init();
#ifdef DEBUG
                  uart_print("start the motor\r\n");
#endif

                  // TODO:
                  //Report the boss that the job is DONE - send email?
                  // probably need esp8266 for this.
               }
               else
               {
                  //sleep for few mins before reading
                  set_sleep_mode(SLEEP_MODE_IDLE);

                  sleep_enable();
                  power_all_disable();
                  //power_spi_disable();
                  //power_timer0_disable();
                  //power_timer2_disable();
                  //power_twi_disable();
#ifdef DEBUG
                  uart_print("goint to sleep\r\n");
#endif
                  //now enter into sleep mode
                  sleep_mode();

                  //wake up
                  sleep_disable();
                  power_all_enable();
#ifdef DEBUG
                  uart_print("wake up from sleep.. yawn! \r\n");
#endif
               }
          }
          else
          {
#ifdef DEBUG
             uart_print("motor is running\r\n");
#endif
             if (timer_overflow >= 7324) // around 2 mins - for 16Mhz speed
             {
                if (TCNT0 > 234)
                {
                   //stop the motor
                   motor_stop();
                   timer0_stop();
                   is_motor_started = 0;
                   timer1_init();
#ifdef DEBUG
                   uart_print("stop the motor\r\n");
#endif
                }
             }
          }
     }

   return 0;
}
