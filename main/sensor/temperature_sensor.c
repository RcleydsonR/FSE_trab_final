#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "dht11.h"
#include "temperature_sensor.h"
#include "mqtt.h"
#include "init.h"

#define TAG "DHT11"

extern SemaphoreHandle_t mqttSemaphoreConn;
extern struct status last_status;
struct dht11_reading dht;
char message[80];

int read_temperature()
{
    dht = DHT11_read();

    if (dht.status == DHT11_OK) {
        last_status.temperature = dht.temperature;
        last_status.humidity = dht.humidity;
        ESP_LOGI(TAG, "{\"Temperatura\": %d, \"Umidade\": %d}", dht.temperature, dht.humidity);
        sprintf(message, "{\"Temperatura\": %d, \"Umidade\": %d}", dht.temperature, dht.humidity);
        return 1;
    }
    else
        ESP_LOGE(TAG, "Erro na leitura do sensor de temperatura");
    return 0;
}

void update_temperature()
{
    int read_status;
    while (1)
    {
        read_status = read_temperature();
        if(read_status){
            mqtt_send_message("v1/devices/me/telemetry", message);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
