#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "i2c.h"
#include "lcd.h"
#include "dht11.h"
#include "init.h"
#include "buzzer.h"
#include "gpio_setup.h"

void init_components()
{
    // i2c_init();
    // ESP_LOGI("I2C", "I2C Inicializado com sucesso");
    // lcd_init();
    // ESP_LOGI("LCD", "LCD Inicializado com sucesso");
    // DHT11_init(DHT11_GPIO);
    // ESP_LOGI("DHT11", "DHT11 Inicializado com sucesso");
    buzzer_init();
    ESP_LOGI("BUZZER", "Buzzer Inicializado com sucesso");

    pinMode(2, GPIO_OUTPUT);

    while (1) {
        digitalWrite(2, 1);
        dash(); dash(); dash();

        space();

        dot(); dash(); dot(); dot();

        space();

        dot(); dash();

        space(); space(); space();

        dash(); dash();

        space();

        dot(); dot(); dash();

        space();

        dash(); dot();

        space();

        dash(); dot(); dot();

        space();

        dash(); dash(); dash();
        digitalWrite(2, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}