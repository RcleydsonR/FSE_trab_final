#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dht11.h"
#include "temperature_sensor.h"
#include "mqtt.h"
#include "init.h"

#define TAG "DHT11"

void read_temperature(void *params)
{
    struct dht11_reading dht;
    char message[80];

    while (1)
    {
        dht = DHT11_read();

        if (dht.status == DHT11_OK) {
            sprintf(message, "{\"Temperatura\": %d, \"Humidade\": %d}", dht.temperature, dht.humidity);
            ESP_LOGI(TAG, "{\"Temperatura\": %d, \"Humidade\": %d}", dht.temperature, dht.humidity);
            if(mqttSemaphoreConn)
                mqtt_send_message("v1/devices/me/telemetry", message);
        }
        else
            ESP_LOGE(TAG, "Erro na leitura do sensor de temperatura");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}