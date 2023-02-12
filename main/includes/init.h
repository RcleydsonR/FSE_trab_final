#ifndef INIT_H_
#define INIT_H_
#include "freertos/semphr.h"
#include <ultrasonic.h>

#define DHT11_GPIO 5
#define ESP_LED_GPIO 2
#define TRIGGER_GPIO 18
#define ECHO_GPIO 19


extern SemaphoreHandle_t wifiSemaphoreConn, mqttSemaphoreConn;
extern ultrasonic_sensor_t sensor;

void init_components();

#endif /* INIT_H_ */
