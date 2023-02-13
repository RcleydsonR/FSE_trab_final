#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/ledc.h"

#include "esp_err.h"
#include "esp_log.h"

#include "buzzer.h"
#include "morse.h"
#include "mqtt.h"
#include "gpio_setup.h"

extern SemaphoreHandle_t mqttSemaphoreConn;

void buzzer_init(int gpio_pin, int buzzer_frequency)
{
    pinMode(gpio_pin, GPIO_OUTPUT);
    ledc_timer_config_t buzzer_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .freq_hz          = buzzer_frequency,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&buzzer_timer));

    ledc_channel_config_t buzzer_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = gpio_pin,
        .duty           = 0,
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&buzzer_channel));
}

void set_duty(int duty_value, char * telemetry_name)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_value);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    
    char message[80];
    sprintf(message, "{\"%s\": %d}", telemetry_name, duty_value == 0 ? 0 : 1);
    if(mqttSemaphoreConn){
        mqtt_send_message("v1/devices/me/telemetry", message);
    }
}

void dot()
{
    set_duty(5000, TELEMETRY_NAME);
    ESP_LOGI("BUZZER", ". acionado");
    vTaskDelay(DOT_DELAY / portTICK_PERIOD_MS);
    set_duty(0, TELEMETRY_NAME);
    vTaskDelay(50 / portTICK_PERIOD_MS);
}

void dash()
{
    set_duty(5000, TELEMETRY_NAME);
    ESP_LOGI("BUZZER", "- acionado");
    vTaskDelay(DASH_DELAY / portTICK_PERIOD_MS);
    set_duty(0, TELEMETRY_NAME);
    vTaskDelay(50 / portTICK_PERIOD_MS);
}

void space()
{
    set_duty(0, TELEMETRY_NAME);
    ESP_LOGI("BUZZER", "  acionado");
    vTaskDelay(SPACE_DELAY / portTICK_PERIOD_MS);
}

void convert_morse_to_sound(char *morse)
{
    // char *morse = convert_string_to_morse(str);
    while (*morse) {
        switch (*morse++)
        {
        case '.':
            dot();
            break;
        case '-':
            dash();
            break;
        default:
            space();
            break;
        }
    }
}
