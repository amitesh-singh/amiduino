#include "stm32f1xx_hal.h"

#define LED_PIN                                GPIO_PIN_13
#define LED_GPIO_PORT                          GPIOC
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOC_CLK_ENABLE()

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Enable HSE Oscillator and configure PLL
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    //RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9; // 72MHz
    RCC_OscInitStruct.PLL.PLLMUL= RCC_PLL_MUL16; //128MHz
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        // Initialization Error
        while (1);
    }

    // Configure clocks: SYSCLK, HCLK, PCLK1, and PCLK2
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        // Initialization Error
        while (1);
    }
}

void blue_pill_led_init()
{
    LED_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef led;
    led.Pin = GPIO_PIN_13;
    led.Mode = GPIO_MODE_OUTPUT_PP;
    led.Pull = GPIO_NOPULL;
    led.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOC, &led);
}

int main()
{
    HAL_Init();
    SystemClock_Config();

    blue_pill_led_init();

    // switch on the led once, gpio13 led is in sinking configuration
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PinState::GPIO_PIN_RESET);
    //note: GPIO_PinState::GPIO_PIN_SET set the pin high
    HAL_Delay(5000);
    while (1) {
        //this would blink the leds
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(1000);
    }
}

extern "C" void SysTick_Handler(void) 
{
  HAL_IncTick();
}