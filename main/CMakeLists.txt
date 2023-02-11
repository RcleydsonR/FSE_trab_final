idf_component_register(SRCS "lcd.c" "i2c.c" "main.c" "temperature_sensor.c"
                    INCLUDE_DIRS "includes"
                    REQUIRES "esp32-DHT11")
