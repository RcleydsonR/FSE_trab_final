#include "stdlib.h"
#include "string.h"

#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "init.h"

#define TAG "NVS"

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

void nvs_save_last_status(void *params)
{
    char *dir = "nvs_storage";
    while (1) {
        nvs_write_int(dir, "temperature", last_status.temperature);
        nvs_write_int(dir, "humidity", last_status.humidity);
        nvs_write_str(dir, "lcd_str", last_status.lcd_str);
        nvs_write_int(dir, "led_esp", last_status.led_esp);
        nvs_write_int(dir, "distance", last_status.distance);
        nvs_write_int(dir, "reverse_gear", last_status.reverse_gear);
        ESP_LOGD(TAG, "Escrito no nvs com sucesso");
        vTaskDelay(50000 / portTICK_PERIOD_MS);
    }
}

void nvs_update_last_status()
{
    char *dir = "nvs_storage";
    size_t tam;

    last_status.temperature = nvs_read_int(dir, "temperature");
    last_status.humidity = nvs_read_int(dir, "humidity");
    strcpy(last_status.lcd_str, nvs_read_str(dir, "lcd_str", &tam));
    last_status.led_esp = nvs_read_int(dir, "led_esp");
    last_status.distance = nvs_read_int(dir, "distance");
    last_status.reverse_gear = nvs_read_int(dir, "reverse_gear");
}