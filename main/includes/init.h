#ifndef INIT_H_
#define INIT_H_
#include "freertos/semphr.h"
#include <ultrasonic.h>

extern ultrasonic_sensor_t sensor;

void init_components();

#endif /* INIT_H_ */
