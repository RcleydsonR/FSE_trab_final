#ifndef LCD_H_
#define LCD_H_

#define SLAVE_ADDRESS_LCD 0x27

void lcd_send_cmd(char cmd);
void lcd_send_data(char data);
void lcd_init();
void lcd_put_cur(int row, int col);
void lcd_send_string(char *str, int sleep_time);
void lcd_clear();
void lcd_morse(void *params);

#endif /* LCD_H_ */