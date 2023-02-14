#ifndef INIT_H_
#define INIT_H_
#include <ultrasonic.h>

extern ultrasonic_sensor_t sensor;

struct status {
    int temperature;
    int humidity;
    char lcd_str[80];
    int led_esp;
    int distance;
    int reverse_gear;
};

void init_energy_mode_components();
void init_battery_mode();

#endif /* INIT_H_ */
