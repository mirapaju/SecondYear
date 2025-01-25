#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void remove_newline(char *str);

//asks a string from the user, prints the length, if user input is stop ->program stops
int main() {
    bool runProgram= true;
    char input[80];

    while (runProgram){
        printf("Enter a string, stop by typing \"stop\".\n");
        fgets(input, 80, stdin);
        remove_newline(input);
        if (strcmp(input, "stop") == 0) {
            runProgram = false;
        }
        else {
            printf("Length of the string: %llu\n", strlen(input));
        }
    }
    printf("Bye bye!\n");
    return 0;
}

//removes the newline character from the user input string
void remove_newline(char *str) {
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}
