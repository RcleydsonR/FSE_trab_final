#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>
#include "esp_log.h"
#include "init.h"
#include "joystick.h"
#include "buzzer.h"
#include "mqtt.h"

#define TAG "HC-SR04"
#define MAX_DISTANCE_CM 500 // 5m max
#define TELEMETRY_BUZZER "Alarme de distância"

float distance;
extern int reverse_gear;
char reverse_gear_led[40];
char distance_attribute[40];

void verifyCarDistance()
{
    sprintf(reverse_gear_led, "{\"reverse_gear_led\": %d}", reverse_gear);
    sprintf(distance_attribute, "{\"Distância traseira\": %.2f}", distance);
    mqtt_send_message("v1/devices/me/attributes", reverse_gear_led);
    mqtt_send_message("v1/devices/me/telemetry", distance_attribute);

    if(reverse_gear){
        if(distance > 10.0 && distance <= 20.0){
            set_duty(3500, TELEMETRY_BUZZER);
            ESP_LOGI(TAG, "Carro esta proximo de algo");
        }
        if(distance <= 10.0){
            set_duty(5000, TELEMETRY_BUZZER);
            ESP_LOGI(TAG, "Carro muito proximo de algo, colisão iminente.");
        }
        else{
            set_duty(0, TELEMETRY_BUZZER);
        }
    }
}

void read_distance(void *params)
{

    while (true)
    {
        esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);
        if (res == ESP_OK){
            distance = distance * 100; // meters to centimeters
            ESP_LOGI(TAG, "Distancia: %0.02f cm", distance);
        }
        else {     
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
        verifyCarDistance();
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
