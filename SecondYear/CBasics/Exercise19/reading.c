#include <stdio.h>
#include <string.h>
#include "menu.h"

void read_filename(char *file, char *prompt) {
    do {
        printf("%s\n", prompt);
        fgets(file, 80, stdin);
    } while (strcmp(file, "\n") == 0);
    if (file[strlen(file) - 1] == '\n') {
        file[strlen(file) - 1] = '\0';
    }
}

int read_range(const char *sort_prompt, int min, int max){
    int number;
    do {
        printf("%s\n", sort_prompt);
        if (scanf("%d", &number) != 1) {
            while (getchar() != '\n');
            printf("Please insert a number!\n");
        }
        else if (number > max || number < min){
            printf("Input out of range!\n");
        }
    } while (number > max || number < min);
    return number;
}
