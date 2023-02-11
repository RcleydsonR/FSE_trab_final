#include "esp_log.h"

#include "i2c.h"
#include "lcd.h"
#include "dht11.h"
#include "init.h"

void init_components()
{
    i2c_init();
    ESP_LOGI("I2C", "I2C Inicializado com sucesso");
    lcd_init();
    ESP_LOGI("LCD", "LCD Inicializado com sucesso");
    DHT11_init(DHT11_GPIO);
    ESP_LOGI("DHT11", "DHT11 Inicializado com sucesso");
}