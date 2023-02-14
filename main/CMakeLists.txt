set(srcs
    "sensor/joystick.c"
    "sensor/ultrasonic_sensor.c"
    "sensor/buzzer.c"
    "sensor/lcd.c"
    "sensor/i2c.c"
    "sensor/temperature_sensor.c"
    "sensor/pwm.c"
    "service/mqtt.c"
    "service/wifi.c"
    "service/nvs.c"
    "utils/morse.c"
    "utils/init.c"
    "utils/gpio_setup.c"
    "utils/parser.c"
    "main.c"
)

idf_component_register(
    SRCS "${srcs}"
    INCLUDE_DIRS "includes"
)
