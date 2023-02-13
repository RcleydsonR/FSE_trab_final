#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_log.h"

#include "init.h"
#include "temperature_sensor.h"
#include "ultrasonic_sensor.h"
#include "i2c.h"
#include "lcd.h"
#include "mqtt.h"
#include "wifi.h"

#define TAG "MAIN"

void app_main(void)
{
#if CONFIG_ESP_ENERGY_MODE
    ESP_LOGI(TAG, "Modo de energia ativado");

#if CONFIG_ESP_MODE_SENDER
    init_energy_mode_components();

    xTaskCreate(&wifi_connected, "Conexao wifi", 2048, NULL, 1, NULL);
    // xTaskCreate(&read_temperature, "Temperatura DHT11", 2048, NULL, 1, NULL);
    // xTaskCreate(&read_distance, "ultrasonic HC-SR04", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(&lcd_morse, "Display LCD", 2048, NULL, 1, NULL);
#endif

#if CONFIG_ESP_MODE_RECEIVER
#endif

#endif

#if CONFIG_ESP_LOW_POWER_MODE

    ESP_LOGI(TAG, "Modo de low power ativado");
    init_battery_mode();

#endif
}
