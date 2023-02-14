#include "stdlib.h"
#include "string.h"

#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "init.h"
#include "lcd.h"
#include "pwm.h"
#include "gpio_setup.h"
#include "mqtt.h"

#define TAG "NVS"
#define DIR "nvs_storage"

static nvs_handle nvs_partition_handler;
extern struct status last_status;

void nvs_init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void open_nvs(char *dir, nvs_open_mode_t open_mode)
{
    esp_err_t ret = nvs_open(dir, open_mode, &nvs_partition_handler);
    if (ret == ESP_ERR_NVS_NOT_FOUND)
        ESP_LOGW(TAG, "Diretorio %s inexistente", dir);
}

int32_t nvs_read_int(char *dir, char *key)
{
    int32_t value = -1;
    open_nvs(dir, NVS_READONLY);
    if (nvs_get_i32(nvs_partition_handler, key, &value) != ESP_OK)
        return -1;

    nvs_close(nvs_partition_handler);
    return value;
}

void nvs_write_int(char *dir, char *key, int value)
{
    open_nvs(dir, NVS_READWRITE);
    ESP_ERROR_CHECK(nvs_set_i32(nvs_partition_handler, key, (int32_t)value));
    nvs_commit(nvs_partition_handler);
    nvs_close(nvs_partition_handler);
}

char * nvs_read_str(char *dir, char *key, size_t *len)
{
    open_nvs(dir, NVS_READONLY);
    if (nvs_get_str(nvs_partition_handler, key, NULL, len) != ESP_OK)
        return "";

    char *value = malloc(*len);
    ESP_ERROR_CHECK(nvs_get_str(nvs_partition_handler, key, value, len));
    nvs_close(nvs_partition_handler);
    return value;
}

void nvs_write_str(char *dir, char *key, char *value)
{
    open_nvs(dir, NVS_READWRITE);
    ESP_ERROR_CHECK(nvs_set_str(nvs_partition_handler, key, value));
    nvs_commit(nvs_partition_handler);
    nvs_close(nvs_partition_handler);
}

void nvs_write_last_status()
{
    nvs_write_int(DIR, "temperature", last_status.temperature);
    nvs_write_int(DIR, "humidity", last_status.humidity);
    nvs_write_str(DIR, "lcd_str", last_status.lcd_str);
    nvs_write_int(DIR, "led_esp", last_status.led_esp);
    nvs_write_int(DIR, "led_pwm", last_status.led_pwm);
    nvs_write_int(DIR, "distance", last_status.distance);
    nvs_write_int(DIR, "reverse_gear", last_status.reverse_gear);
    ESP_LOGI(TAG, "Escrito no nvs com sucesso");
}

void nvs_save_last_status(void *params)
{
    while (1) {
        nvs_write_last_status();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void nvs_update_last_status()
{
    size_t tam;
    last_status.temperature = nvs_read_int(DIR, "temperature");
    last_status.humidity = nvs_read_int(DIR, "humidity");
    strcpy(last_status.lcd_str, nvs_read_str(DIR, "lcd_str", &tam));
    last_status.led_esp = nvs_read_int(DIR, "led_esp");
    last_status.led_pwm = nvs_read_int(DIR, "led_pwm");
    last_status.distance = nvs_read_int(DIR, "distance");
    last_status.reverse_gear = nvs_read_int(DIR, "reverse_gear");

    char mqtt_message[100];

#if CONFIG_ESP_MODE_MORSE
    lcd_send_string_silent(last_status.lcd_str);
    sprintf(mqtt_message, "{\"lcd_str\": %s}", last_status.lcd_str);
#elif CONFIG_ESP_MODE_TEMPERATURE
    digitalWrite(ESP_LED_GPIO, last_status.led_esp);
    sprintf(mqtt_message, "{\"led\": %d}", last_status.led_esp);
    set_pwm_duty(last_status.led_pwm);
#endif
    ESP_LOGI(TAG, "Informacoes foram atualizadas.");
#if CONFIG_ESP_ENERGY_MODE
    mqtt_send_message("v1/devices/me/attributes", mqtt_message);
#endif
}