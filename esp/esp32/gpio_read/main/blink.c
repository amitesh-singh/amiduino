/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO
#define READ_GPIO CONFIG_BLINK2_GPIO

void blink_gpio5_task(void *pvParameter)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    printf("BLINK GPIO5 TASK\n");
	fflush(stdout);

    while(1)
    {
        /* Blink off (output low) */
        gpio_set_level(BLINK_GPIO, 0);
        printf("LED 5 is off\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO, 1);
        printf("LED 5 is on\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
		fflush(stdout);
    }
}

void blink_gpio17_task(void *pvParameter)
{
	printf("BLINK GPIO17 TASK\n");
	fflush(stdout);

	gpio_pad_select_gpio(READ_GPIO);
	    /* Set the GPIO as a push/pull output */
	 gpio_set_direction(READ_GPIO, GPIO_MODE_OUTPUT);
	while (1)
	{
		 gpio_set_level(READ_GPIO, 0);
		 printf("LED 17 is off\n");
		 vTaskDelay(1000 / portTICK_RATE_MS);
		 /* Blink on (output high) */
		 gpio_set_level(READ_GPIO, 1);
		 printf("LED 17 is on\n");
		 vTaskDelay(1000 / portTICK_RATE_MS);
		 fflush(stdout);
	}
}

void app_main()
{
    nvs_flash_init();
    system_init();
    xTaskCreate(&blink_gpio5_task, "blink GPIO5_task", 512, NULL, 5, NULL);
    printf("inside app_main...");
    xTaskCreate(&blink_gpio17_task, "blink GPIO17 task", 512, NULL, 5, NULL);
}















