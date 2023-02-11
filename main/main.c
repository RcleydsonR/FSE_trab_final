#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "temperature_sensor.h"

#define TAG "MAIN"

void app_main(void)
{
#if CONFIG_ESP_MODE_SENDER
    ESP_LOGI(TAG, "Iniciando leitura de temperatura");
    xTaskCreate(&read_temperature, "Temperatura DHT11", 2048, NULL, 1, NULL);
#endif
}
