#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "grade_calculator.h"

void read_string(char *name, const char *prompt) {
    do {
        printf("%s", prompt);
        if (fgets(name, NAME_SIZE, stdin) != NULL) {
            if (name[strlen(name) - 1] != '\n') {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
            }
            if (name[strlen(name) - 1] == '\n') {
                name[strlen(name) - 1] = '\0';
            }
            if (name[0] == '\0'){
                printf("Error! Input didn't contain any characters! \n");
            }
        } else {
            printf("Error reading the input.\n");
        }
    } while (name[0] == '\0');
}

//read integers in range with or without upper limit
int read_int(const char *prompt, int min_limit, int max_limit) {
    char input[NAME_SIZE];
    int number;
    bool invalidInput = true;

    while (invalidInput) {
        read_string(input, prompt);
        if (sscanf(input, "%d", &number) != 1) {
            printf("Please enter numbers!\n");
        }
        else if (min_limit < 0 && number <= 0) {
            printf("Please enter a positive number!\n");
        }
        else if (number < min_limit) {
            printf("Please enter a number greater than or equal to %d!\n", min_limit);
        }
        else if (max_limit != -1 && number > max_limit) {
            printf("Input too high, maximum score is %d!\n", max_limit);
        }
        else {
            invalidInput = false;
        }
    }
    return number;
}