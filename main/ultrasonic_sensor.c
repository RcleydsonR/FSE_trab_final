#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>
#include "esp_log.h"
#include "init.h"

#define TAG "HC-SR04"
#define MAX_DISTANCE_CM 500 // 5m max

void read_distance(void *pvParameters)
{
    float distance;

    while (true)
    {
        esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);
        if (res != ESP_OK)
        {
            printf("Error %d: ", res);
            switch (res)
            {
            case ESP_ERR_ULTRASONIC_PING:
                ESP_LOGE(TAG, "Não foi possível executar o ping.\n");
                break;
            case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                ESP_LOGE(TAG, "Tempo limite de ping(dispositivo não encontrado)\n");
                break;
            case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                ESP_LOGE(TAG, "Tempo limite de eco(disancia muito grande)\n");
                break;
            default:
                ESP_LOGE(TAG, "%s\n", esp_err_to_name(res));
            }
        }
        else
            ESP_LOGI(TAG, "Distancia: %0.02f cm", distance * 100);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
