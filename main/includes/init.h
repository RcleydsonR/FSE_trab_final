#ifndef INIT_H_
#define INIT_H_
#include "freertos/semphr.h"

#define DHT11_GPIO 5

extern SemaphoreHandle_t wifiSemaphoreConn, mqttSemaphoreConn;

void init_components();

#endif /* INIT_H_ */