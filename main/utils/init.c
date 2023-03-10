#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "esp32/rom/uart.h"
#include "driver/rtc_io.h"

#include "esp_sleep.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "i2c.h"
#include "lcd.h"
#include "dht11.h"
#include "init.h"
#include "nvs.h"
#include "buzzer.h"
#include "gpio_setup.h"
#include "temperature_sensor.h"
#include "wifi.h"
#include "pwm.h"
#include "mqtt.h"

#define TAG "SENSOR_INIT"
#define LOW_MODE_TAG "LOW_MODE"
#define SECOND_IN_MICROSECOND 1000000

QueueHandle_t lcdQueue;
SemaphoreHandle_t wifiSemaphoreConn, mqttSemaphoreConn;
ultrasonic_sensor_t sensor = {
    .trigger_pin = TRIGGER_GPIO,
    .echo_pin = ECHO_GPIO
};

struct status last_status = {
    .temperature = -1,
    .humidity = -1,
    .lcd_str = "",
    .led_esp = -1,
    .led_pwm = -1,
    .distance = -1,
    .reverse_gear = -1
};

void init_energy_mode_components()
{
    nvs_init();
    ESP_LOGI("NVS", "NVS Inicializado com sucesso");

    mqttSemaphoreConn = xSemaphoreCreateBinary();
    wifiSemaphoreConn = xSemaphoreCreateBinary();

    wifi_start();
    pinMode(ESP_LED_GPIO, GPIO_OUTPUT);

#if CONFIG_ESP_MODE_CAR
    ultrasonic_init(&sensor);
    ESP_LOGI("HC-SR04", "SR04 Inicializado com sucesso");
    buzzer_init(26, 500);
    ESP_LOGI(TAG, "Buzzer Inicializado com sucesso");
#elif CONFIG_ESP_MODE_MORSE
    lcdQueue = xQueueCreate(4, 16);
    i2c_init();
    ESP_LOGI(TAG, "I2C Inicializado com sucesso");
    lcd_init();
    ESP_LOGI(TAG, "LCD Inicializado com sucesso");
    buzzer_init(33, 400);
    ESP_LOGI(TAG, "Buzzer Inicializado com sucesso");
#elif CONFIG_ESP_MODE_TEMPERATURE
    init_pwm();
    ESP_LOGI(TAG, "LED_7_FLASH Inicializado com sucesso");
    DHT11_init(DHT11_GPIO);
    ESP_LOGI(TAG, "DHT11 Inicializado com sucesso");
#endif
    nvs_update_last_status();
}

void init_battery_mode()
{
  nvs_init();
  ESP_LOGI("NVS", "NVS Inicializado com sucesso");
  nvs_update_last_status();

  // Configuração da GPIO para o botão de entrada
  pinMode(ESP_BUTTON_GPIO, GPIO_INPUT);
#if CONFIG_ESP_MODE_TEMPERATURE
    DHT11_init(DHT11_GPIO);
    ESP_LOGI(TAG, "DHT11 Inicializado com sucesso");
#endif
  // Habilita o botão para acordar a placa
  gpio_wakeup_enable(ESP_BUTTON_GPIO, GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();

  // Configurando o Sleep Timer (30s de sleep)
  esp_sleep_enable_timer_wakeup(5 * SECOND_IN_MICROSECOND);

  int64_t time_before_sleep, time_after_sleep;

  while(1)
  {
    read_temperature();
    nvs_write_last_status();
    if (rtc_gpio_get_level(ESP_BUTTON_GPIO) == 0)
    {
      ESP_LOGI(LOW_MODE_TAG, "O botão da esp me acordou!");
      ESP_LOGI(LOW_MODE_TAG, "Aguardando soltar o botão ...");
      do
      {
          vTaskDelay(pdMS_TO_TICKS(10));
      } while (rtc_gpio_get_level(ESP_BUTTON_GPIO) == 0);
    }

    ESP_LOGI(LOW_MODE_TAG, "Entrando em modo Light Sleep");
    
    // Configura o modo sleep somente após completar a escrita na UART para finalizar o LOG
    uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

    time_before_sleep = esp_timer_get_time();

    // Entra em modo Light Sleep
    esp_light_sleep_start();

    time_after_sleep = esp_timer_get_time();

    ESP_LOGI(LOW_MODE_TAG, "Dormi por %.2f segundos", ((time_after_sleep - time_before_sleep) / (float)(SECOND_IN_MICROSECOND)));
    if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER)
      ESP_LOGI(LOW_MODE_TAG, "O timer me acordou!");
      // colocar logica de atualizacao dos estados via mqtt
  }
}
