#include "string.h"
#include "stdlib.h"

#include "esp_log.h"

static char morse_table[36][5] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.",
    "--.", "....", "..", ".---", "-.-", ".-..",
    "--", "-.", "---", ".--.", "--.-", ".-.",
    "...", "-", "..-", "...-", ".--", "-..-",
    "-.--", "--..", "-----", ".----", "..---", "...--",
    "....-", ".....", "-....", "--...", "---..", "----."
};

char* convert_char_to_morse(char c)
{
    int morse_pos = c;
    morse_pos = morse_pos > 90 ? morse_pos - (32) : morse_pos;
    morse_pos -= 65;
    return morse_table[morse_pos < 0 ? morse_pos + 43 : morse_pos];
}

char* convert_string_to_morse(char *str)
{
    char *result = (char *) malloc(82); 
    memset(result, 0, 82);
    strcpy(result, "");

    while(*str) {
        char c = *str++;
        if (c == ' ')
            strcat(result, "/");
        else {
            strcat(result, convert_char_to_morse(c));
        }
        
        if (*str)
            strcat(result, " ");
    }
    ESP_LOGI("MORSE", "Morse calculado: %s", result);
    return result;
}

void convert_morse_to_string() {}