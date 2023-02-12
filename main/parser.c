#include <stdio.h>
#include <string.h>
#include "esp_log.h"

#include "mqtt.h"
#include "cJSON.h"
#include "gpio_setup.h"

void handle_broker_message(char * json_message)
{
    cJSON *json = cJSON_Parse(json_message);
    char *command = cJSON_GetObjectItem(json, "method")->valuestring;

    // char *decoded_json = cJSON_Print(json);
    // ESP_LOGI("CJSON", "A mensagem json recebida é: %s", decoded_json);
    
    if (strcmp(command, "acionarLed") == 0){
        char led[40];
        led_state = !led_state;

        sprintf(led, "{\"led\": %d}", led_state);

        digitalWrite(ESP_LED_GPIO, led_state);
        mqtt_send_message("v1/devices/me/attributes", led);
    }
}
