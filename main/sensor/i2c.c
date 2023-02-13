#include "esp_log.h"

#include "driver/i2c.h"

#include "i2c.h"

void i2c_init() {
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = PIN_NUN_SDA,
        .scl_io_num = PIN_NUN_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_HOST, &i2c_conf));
    i2c_driver_install(I2C_HOST, I2C_MODE_MASTER, 0, 0, 0);
}