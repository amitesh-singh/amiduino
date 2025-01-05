#include "stm32f1xx_hal.h"

#define LED_PIN                                GPIO_PIN_13
#define LED_GPIO_PORT                          GPIOC
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOC_CLK_ENABLE()


void blue_pill_led_init()
{
    LED_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef led;
    led.Pin = GPIO_PIN_13;
    led.Mode = GPIO_MODE_OUTPUT_PP;
    led.Pull = GPIO_PULLUP;
    led.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOC, &led);
}

void my_delay()
{
    for (int i = 0; i < 100000; ++i) {
        __asm__ volatile ("nop");
    }
}
int main()
{
    HAL_Init();
    blue_pill_led_init();

    while (1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(1000);
    }
}

void SysTick_Handler(void) {
  HAL_IncTick();
}