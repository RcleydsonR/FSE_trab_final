#ifndef _GPIO_SETUP
#define _GPIO_SETUP

#include "driver/gpio.h"

#define GPIO_OUTPUT          0
#define GPIO_INPUT           1
#define GPIO_INPUT_PULLUP    2
#define GPIO_INPUT_PULLDOWN  3

#define ESP_LED 2

extern int led_state;
void pinMode(gpio_num_t pin, int mode);
void digitalWrite(gpio_num_t pin, uint32_t level);
uint32_t digitalRead(gpio_num_t pin);

#endif
