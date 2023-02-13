#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "esp_log.h"

#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"

#include "gpio_setup.h"

#define TAG "GPIO_SETUP"

int led_state = 0;
adc_oneshot_unit_handle_t adc1_handle;
adc_oneshot_unit_init_cfg_t init_config1;

void adc_init(adc_unit_t adc_unit)
{
    init_config1.unit_id = adc_unit;
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
}

void adc_config_pin(adc_channel_t channel)
{
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, channel, &config));
}

void adc_deinit()
{
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc1_handle));
}

void pinMode(gpio_num_t pin, int mode){
    gpio_config_t io_conf = {};
    switch (mode)
    {
    case GPIO_OUTPUT:
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << pin);
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.pull_down_en = 0;
        io_conf.pull_up_en = 0;
        gpio_config(&io_conf);
        break;
    case GPIO_INPUT:
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL << pin);
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.pull_down_en = 0;
        io_conf.pull_up_en = 0;
        gpio_config(&io_conf);
        break;
    case GPIO_INPUT_PULLUP:
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL << pin);
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.pull_down_en = 0;
        io_conf.pull_up_en = 1;
        gpio_config(&io_conf);
        break;
    case GPIO_INPUT_PULLDOWN:
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL << pin);
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.pull_down_en = 1;
        io_conf.pull_up_en = 0;
        gpio_config(&io_conf);
        break;
    case GPIO_ANALOG:
        adc_config_pin(pin);
        break;
    default:
        ESP_LOGW(TAG, "Unable to configure pin %d", pin);
        break;
    }
}

void digitalWrite(gpio_num_t pin, uint32_t level){
    gpio_set_level(pin, level);
}

uint32_t digitalRead(gpio_num_t pin){
    return gpio_get_level(pin);
}

int analogRead(adc_channel_t channel){
    int adc_raw;
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, channel, &adc_raw));
    return adc_raw;
}
