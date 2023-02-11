#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dht11.h"
#include "temperature_sensor.h"

#define TEMP_DHT11_GPIO 5
#define TAG "TEMPERATURE SENSOR (DHT11)"

void read_temperature(void *params)
{
    DHT11_init(TEMP_DHT11_GPIO);
    struct dht11_reading dht;

    while (1)
    {
        dht = DHT11_read();

        if (dht.status == DHT11_OK)
            ESP_LOGI(TAG, "Temperatura: %d, Humidade: %d", dht.temperature, dht.humidity);
        else
            ESP_LOGE(TAG, "Erro na leitura do sensor de temperatura");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}