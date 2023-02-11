#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_log.h"

#include "temperature_sensor.h"
#include "i2c.h"
#include "lcd.h"

#define TAG "MAIN"

void app_main(void)
{
#if CONFIG_ESP_MODE_SENDER
    ESP_LOGI(TAG, "Iniciando leitura de temperatura");
    // xTaskCreate(&read_temperature, "Temperatura DHT11", 2048, NULL, 1, NULL);
    i2c_init();
    lcd_init();

    while (1) {
        lcd_clear();
        lcd_put_cur(0, 0);
        lcd_send_string("Teste 1234", 100);

        lcd_put_cur(1, 0);
        lcd_send_string("ASFoancaopsincas", 100);

        lcd_clear();
        lcd_put_cur(0, 0);
        lcd_send_string("AAAAAAAAA", 100);

        lcd_put_cur(1, 0);
        lcd_send_string("12421.123124", 100);
    }
#endif
}
