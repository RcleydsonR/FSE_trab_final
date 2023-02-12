#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_GPIO 33
#define DOT_DELAY 140
#define DASH_DELAY 300
#define SPACE_DELAY 400

void buzzer_init();
void dot();
void dash();
void space();
void convert_morse_to_sound(char *str);

#endif /* BUZZER_H_ */