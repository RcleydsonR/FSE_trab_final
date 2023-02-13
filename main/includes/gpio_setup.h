#ifndef GPIO_SETUP_H_
#define GPIO_SETUP_H_

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

#define GPIO_OUTPUT          0
#define GPIO_INPUT           1
#define GPIO_INPUT_PULLUP    2
#define GPIO_INPUT_PULLDOWN  3
#define GPIO_ANALOG          4

#define ESP_BUTTON_GPIO 0
#define DHT11_GPIO 5
#define ESP_LED_GPIO 2
#define TRIGGER_GPIO 18
#define ECHO_GPIO 19

extern int led_state;

void adc_init();
void adc_config_pin(adc_channel_t channel);
void adc_deinit();
void pinMode(gpio_num_t pin, int mode);
void digitalWrite(gpio_num_t pin, uint32_t level);
uint32_t digitalRead(gpio_num_t pin);
int analogRead(adc_channel_t channel);

#endif /* _GPIO_SETUP_H_ */
