#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "esp_log.h"

#include "i2c.h"
#include "lcd.h"
#include "dht11.h"
#include "init.h"
#include "buzzer.h"
#include "gpio_setup.h"
#include "wifi.h"

SemaphoreHandle_t wifiSemaphoreConn, mqttSemaphoreConn;
  ultrasonic_sensor_t sensor = {
      .trigger_pin = TRIGGER_GPIO,
      .echo_pin = ECHO_GPIO
  };

void init_components()
{
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI("NVS_FLASH", "NVS_FLASH Inicializado com sucesso");
    mqttSemaphoreConn = xSemaphoreCreateBinary();
    wifiSemaphoreConn = xSemaphoreCreateBinary();

    wifi_start();

    // i2c_init();
    // ESP_LOGI("I2C", "I2C Inicializado com sucesso");
    // lcd_init();
    // ESP_LOGI("LCD", "LCD Inicializado com sucesso");
    DHT11_init(DHT11_GPIO);
    ESP_LOGI("DHT11", "DHT11 Inicializado com sucesso");
    // buzzer_init();
    // ESP_LOGI("BUZZER", "Buzzer Inicializado com sucesso");

    pinMode(2, GPIO_OUTPUT);

    // Exemplo de string
    // while (1) {
    //     digitalWrite(2, 1);
    //     convert_morse_to_sound(convert_string_to_morse("ascaiskjcnasc"));
    //     digitalWrite(2, 0);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }

    ultrasonic_init(&sensor);
    ESP_LOGI("HC-SR04", "SR04 Inicializado com sucesso");
}
