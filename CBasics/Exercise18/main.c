#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int read_range(char *prompt, int max_value);

int main() {
    bool program_running =true;
    int number;
    int random;
    int shifted_number;
    srand(time(NULL));

    while (program_running){
        char prompt[] = "Please enter a number between 0 and 15 or a negative number to stop.";
        number = read_range(prompt,15);
        if (number < 0) {
            program_running = false;
        }
        else {
            random = rand() % 256;
            printf("Random number in hexadecimal: 0x%02X\n", random);
            shifted_number = random >> number;
            shifted_number &= 0x3F;
            printf("Shifted number in hexadecimal: 0x%02X\n", shifted_number);
        }
    }
    printf("Byebye!");
    return 0;
}

int read_range(char *prompt, int max_value){
    int number;
    do {
        printf("%s\n", prompt);
        if (scanf("%d", &number) != 1) {
            while (getchar() != '\n');
            printf("Please insert a number!\n");
        }
        else if (number > max_value){
            printf("Input out of range!\n");
        }
    } while (number > max_value);
    return number;
}
