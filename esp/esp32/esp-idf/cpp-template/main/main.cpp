#include <stdio.h>

#include <esp_log.h>

//For C++, extern "C" is required due to name mangling
extern "C" void app_main(void)
{
    printf("Hello World");
    ESP_LOGI("hey", "hey man");
}
