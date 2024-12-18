#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

static volatile uint32_t system_ms = 0;

static void gpio_setup(void)
{
   // LED2 - PD3
   rcc_periph_clock_enable(RCC_GPIOD);
   gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
                   GPIO_PUPD_NONE, GPIO3);
   //LED1 - PC0
   rcc_periph_clock_enable(RCC_GPIOC);
   gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT,
                   GPIO_PUPD_NONE, GPIO0);
}

static void systick_setup(void)
{
   // 16MHz/8 = 2000000
   systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

   // 2000000/2000 = 1000
   /* SysTick interrupt every N clock pulses: set reload to N-1 */
   systick_set_reload(1999);

   systick_interrupt_enable();

   /* Start counting. */
   systick_counter_enable();
}

void sys_tick_handler(void)
{
   system_ms++;
}

static void delay(uint32_t delay_ms)
{
   uint32_t future_ms = system_ms + delay_ms;
   while (future_ms > system_ms);
}

int main(void)
{
   //default speed is 16MHz set by libopencm3
   gpio_setup();
   systick_setup();

   while (1)
     {
        gpio_set(GPIOC, GPIO0);
        delay(400);
        gpio_clear(GPIOC, GPIO0);
        gpio_set(GPIOD, GPIO3);
        delay(400);
        gpio_clear(GPIOD, GPIO3);
     }

   return 0;
}
