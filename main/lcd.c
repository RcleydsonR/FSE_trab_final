#include "esp_log.h"

#include "driver/i2c.h"

#include "freertos/task.h"

#include "i2c.h"
#include "lcd.h"

static char *TAG = "LCD";

void lcd_send_cmd(char cmd)
{
	esp_err_t err;
	char data_u, data_l;
	uint8_t data_t[4];

	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);

	data_t[0] = data_u|0x0C;
	data_t[1] = data_u|0x08;
	data_t[2] = data_l|0x0C;
	data_t[3] = data_l|0x08;

	err = i2c_master_write_to_device (I2C_HOST, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
	if (err != 0)
		ESP_LOGE(TAG, "Error no. %d in command", err);
}

void lcd_send_data(char data)
{
	esp_err_t err;
	char data_u, data_l;
	uint8_t data_t[4];

	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);

	data_t[0] = data_u|0x0D;
	data_t[1] = data_u|0x09;
	data_t[2] = data_l|0x0D;
	data_t[3] = data_l|0x09;

	err = i2c_master_write_to_device (I2C_HOST, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
	if (err != 0)
		ESP_LOGE(TAG, "Error no. %d in command", err);
}

void lcd_init()
{
    // Inicializacao 4 bit
	vTaskDelay(6250 / portTICK_PERIOD_MS);
	lcd_send_cmd (0x30);
	vTaskDelay(562 / portTICK_PERIOD_MS);
	lcd_send_cmd (0x30);
	vTaskDelay(25 / portTICK_PERIOD_MS);
	lcd_send_cmd (0x30);
	vTaskDelay(25 / portTICK_PERIOD_MS);
	lcd_send_cmd (0x20);
	vTaskDelay(25 / portTICK_PERIOD_MS);

  	// Inicializacao do Display
	lcd_send_cmd (0x28);
	vTaskDelay(125 / portTICK_PERIOD_MS);
	lcd_send_cmd (0x08);
	vTaskDelay(125 / portTICK_PERIOD_MS);
	lcd_send_cmd (0x01);
	vTaskDelay(250 / portTICK_PERIOD_MS);
	lcd_send_cmd (0x06);
	vTaskDelay(125 / portTICK_PERIOD_MS);
	lcd_send_cmd (0x0C);
	vTaskDelay(250 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "LCD inicializado com sucesso");
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd(col);
}

void lcd_send_string(char *str, int sleep_time)
{
	while (*str) {
		lcd_send_data (*str++);
		vTaskDelay(sleep_time / portTICK_PERIOD_MS);
	}
}

void lcd_clear()
{
	lcd_send_cmd (0x01);
	vTaskDelay(625 / portTICK_PERIOD_MS);
}