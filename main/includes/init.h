#ifndef INIT_H_
#define INIT_H_
#include "freertos/semphr.h"
#include <ultrasonic.h>

extern SemaphoreHandle_t wifiSemaphoreConn, mqttSemaphoreConn;
extern ultrasonic_sensor_t sensor;

void init_components();

#endif /* INIT_H_ */
