#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>


static void my_delay()
{
    int i = 72e6/2/4;

    while (i > 0)
    {
        i--;
        __asm__("nop");
    }
}

int main()
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOC);

    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    while (1)
    {
        gpio_toggle(GPIOC, GPIO13);
        my_delay();
    }

    return 0;
}
