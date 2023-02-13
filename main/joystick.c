#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"

#include "esp_adc/adc_oneshot.h"
#include "gpio_setup.h"
#include "joystick.h"

int x, y, btn;
int reverse_gear = 0;

void read_joystick(void *params)
{
    int can_trigger_reverse_gear = 0;
    int last_btn = 1;

    adc_init(ADC_UNIT_1);

    adc_config_pin(JOYSTICK_X); 
    adc_config_pin(JOYSTICK_Y);

    pinMode(JOYSTICK_BOTAO, GPIO_INPUT_PULLDOWN);
    
    while (true)
    {
        printf("%d",JOYSTICK_BOTAO);
        x = analogRead(JOYSTICK_X);
        y = analogRead(JOYSTICK_Y);
        btn = digitalRead(JOYSTICK_BOTAO);

        x = x - 1690;
        y = y - 1730;

        if(btn == 0 && last_btn == 1){
            can_trigger_reverse_gear = !can_trigger_reverse_gear;
            if(can_trigger_reverse_gear == 0)
                reverse_gear = 0;
        }

        if(can_trigger_reverse_gear == 1 && y < -1500)
                reverse_gear = 1;

        printf("Posição X: %.3d \t Posição Y: %.3d | Botão: %d\n", x, y, btn);
        last_btn = btn;
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }

}

