idf_component_register(SRCS "init.c" "lcd.c" "i2c.c" "main.c" "temperature_sensor.c" "gpio_setup.c"
                    INCLUDE_DIRS "includes"
                    REQUIRES "esp32-DHT11")
