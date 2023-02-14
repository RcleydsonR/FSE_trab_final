#include <stdio.h>
#include <string.h>
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "mqtt.h"
#include "init.h"
#include "cJSON.h"
#include "gpio_setup.h"
#include "joystick.h"
#include "pwm.h"

extern struct status last_status;
extern QueueHandle_t lcdQueue;
extern int reverse_gear;
extern int led_state;

void handle_broker_message(char * json_message)
{
    cJSON *json = cJSON_Parse(json_message);
    char *command = cJSON_GetObjectItem(json, "method")->valuestring;
    cJSON *params= cJSON_GetObjectItemCaseSensitive(json, "params");

    char *decoded_json = cJSON_Print(json);
    ESP_LOGI("CJSON", "A mensagem json recebida é: %s", decoded_json);
    
    if (strcmp(command, "acionarLed") == 0) {
        char led[40];
        led_state = !led_state;

        sprintf(led, "{\"led\": %d}", led_state);
        last_status.led_esp = led_state;

        digitalWrite(ESP_LED_GPIO, led_state);
        mqtt_send_message("v1/devices/me/attributes", led);
    }
    else if (strcmp(command, "escreverLcd") == 0) {
        char *str= cJSON_GetObjectItem(params, "lcd_str")->valuestring;
        if (strlen(str) <= 16)
            xQueueSend(lcdQueue, str, portMAX_DELAY);
    }
    else if (strcmp(command, "setPWM") == 0) {
        int pwm = cJSON_GetObjectItem(json, "params")->valueint;
        set_pwm_duty(pwm);
    }
    else if (strcmp(command, "acionarRe") == 0) {
        char reverse_gear_led[40];
        reverse_gear = !reverse_gear;

        sprintf(reverse_gear_led, "{\"reverse_gear_led\": %d}", reverse_gear);

        mqtt_send_message("v1/devices/me/attributes", reverse_gear_led);
    }
}
