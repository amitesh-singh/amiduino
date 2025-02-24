/* Hello World Example

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
//#define portTICK_RATE_MS portTICK_PERIOD_MS

void hello_task(void *pvParameter)
{
   //output goes to serial interface 115200 baud rate.
    printf("Hello world!\n");
    for (int i = 10; i >= 0; i--)
    {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    printf("Restarting now.\n");
    fflush(stdout);
    system_restart();
}

void app_main()
{
    nvs_flash_init();
    system_init();
    //2048 is the stack size, 5 - priority, before that, its pvParameter
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
}
