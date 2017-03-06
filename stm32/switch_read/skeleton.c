#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void my_delay_1( void )
{
   int i = 1000;

   while( i > 0 )
     {
        i--;
        __asm__( "nop" );
     }
}

int main( void )
{
   uint16_t new_state = 0, old_state = -1; //0xFF
   //set STM32 to 72 MHz
   rcc_clock_setup_in_hse_8mhz_out_72mhz();
   // Enable GPIOC clock
   rcc_periph_clock_enable( RCC_GPIOC );
   //Set GPIO13 (inbuild led connected) to 'output push-pull'
   gpio_set_mode( GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13 );
   
   // make GPIO14 to read, GPIO_CNF_INPUT_PULL_UPDOWN -> enables internal
   // pullup and pullup
   gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                 GPIO14);
   // enable pullup 
   gpio_set(GPIOC, GPIO14);

   // enable pulldown
   // gpio_set(GPIOC, GPI14);

   while( 1 )
     {
       new_state = gpio_get(GPIOC, GPIO14);
       // Read the current value of the given GPIO port. Only the lower 16 bits 
       // contain valid pin data.
       // or new_state = gpio_port_read(GPIOC) & GPIO14
       my_delay_1();
       if (new_state != old_state)
       {
         gpio_toggle(GPIOC, GPIO13);
         old_state = new_state;
       }
     }
}
