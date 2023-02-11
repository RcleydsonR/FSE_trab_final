#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_GPIO 33
#define DOT_DELAY 70
#define DASH_DELAY 150
#define SPACE_DELAY 200

void buzzer_init();
void dot();
void dash();
void space();
void convert_morse_to_sound(char *str);

#endif /* BUZZER_H_ */