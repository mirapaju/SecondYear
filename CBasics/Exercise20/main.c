#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "debug.h"

int main() {
    char prompt[] = "Insert a number between 0-4: ";
    int min =0;
    int max =4;
    int level_choice, random_level;

    level_choice = read_range(prompt, min, max);
    set_debug_level(level_choice);

    srand(time(NULL));

    for (int i=0; i<5; i++){
        random_level = rand() %5;
        dprintf(random_level, "Debug message nr%d, random debuglevel %d.\n", i+1, random_level);
    }
    return 0;
}

int read_range(char *prompt, int min_value, int max_value){
    int number;
    do {
        printf("%s\n", prompt);
        if (scanf("%d", &number) != 1) {
            while (getchar() != '\n');
            printf("Please insert a number!\n");
        }
        else if (number > max_value || number < min_value){
            printf("Input out of range!\n");
        }
    } while (number > max_value || number < min_value);
    return number;
}
