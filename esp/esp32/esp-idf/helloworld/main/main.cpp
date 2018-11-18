
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include <cstring>


esp_err_t event_handler(void *ctx, system_event_t *event)
{
   return ESP_OK;
}

extern "C" void app_main(void)
{
   nvs_flash_init();
   tcpip_adapter_init();
   system_init();
   ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
   //set up wifi
   wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
   ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
   ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
   ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
   wifi_config_t sta_config = {0};
   strcpy((char *)sta_config.sta.ssid, "POOJA");
   strcpy((char *)sta_config.sta.password, "poojasingh");
   ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
   ESP_ERROR_CHECK( esp_wifi_start() );
   ESP_ERROR_CHECK( esp_wifi_connect() );

   gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
   int level = 0;
   printf("Connecting to WiFi.\r\n");
   while (true)
    {
        printf("It blinks\r\n");
        gpio_set_level(GPIO_NUM_4, level);
        level = !level;
        vTaskDelay(300 / portTICK_PERIOD_MS);
   }
}

