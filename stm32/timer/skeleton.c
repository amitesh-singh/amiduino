#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>

static void clock_setup(void)
{
  rcc_clock_setup_in_hse_8mhz_out_24mhz(); //set stm32 to 72 Mhz 
}

//tim1 - advanced control timer
static void timer1_setup(void)
{
  //enable the timer1 
  rcc_periph_clock_enable(RCC_TIM1);

  //reset a timer. 
  timer_reset(TIM1);
  
  // NVIC Enable Interrupt
  // Enables a user interrupt.
  nvic_enable_irq(NVIC_TIM1_UP_IRQ);

  // set the timer mode.
  // timer_set_mode(timer_peripheral, clock_div, alignment, direction)
  // clock_div -> Clock divider ratio (to form the sampling clock for the input filters,
  // and the dead-time clock in the advanced timers 1 and 8)
  // alignment => center/Edge alignment
  // direction - 
  timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                 TIM_CR1_DIR_UP);
  // set the value for the timer prescalar
  // The timer clock is prescaled by the 16 bit scale value plus 1
  // Any integer between 1 and 65536
  timer_set_prescaler(TIM1, ((72e6)/2)/10000); //1 tick = 100 us 
  //Set timer period 
  // Specify the timer period in the auto-reload register.
  // period Unsigned int32. Period in counter clock ticks.
  timer_set_period(TIM1, 20000);
  //Enable the Timer to Run Continuously.
  //timer_peripheral Unsigned int32. Timer register address base
  timer_continuous_mode(TIM1);
  // updaate interrupt enable
  TIM_DIER(TIM1) |= TIM_DIER_UIE;
  //Enable the timer to start counting.
  // This should be called after the timer initial configuration has been completed.
  //
  timer_enable_counter(TIM1);

  // stop the timer from counting.
  //timer_disable_counter(TIM1);
}

static void gpio_setup(void)
{
  //enable GPIOC clock
  rcc_periph_clock_enable(RCC_GPIOC);
  //inbuild led is at 13 port - set it to 'output push-poll'
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

void tim1_up_isr(void)
{
  gpio_toggle(GPIOC, GPIO13);
  TIM_SR(TIM1) &= ~TIM_SR_UIF;
}

int main( void )
{
  clock_setup();
  gpio_setup();

  timer1_setup();

   while( 1 )
     {
        //do nothing
     }
}
