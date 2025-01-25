#include <stdio.h>

int readRange(int low, int high);

/*"dicegame", x rounds. Based on user input the function calculates that
the computer wins by +1, unless the user input is the highest possible die value (=tie) */
int main() {
    int dieOutcome;
    int minValue = 1;
    int maxValue = 6;
    int rounds = 3;

    printf("Lets Play!\nRoll a die and enter your result.\n");

    for (int i=0; i < rounds; i++){
        dieOutcome = readRange(minValue,maxValue);
        (dieOutcome == maxValue) ? printf("I got 6. It is a tie!\n") : printf("I got %d. I win!\n", dieOutcome + 1);
    }
    printf("Better luck next time. Bye!\n");
    return 0;
}

//function for checking that the user input is between the desired range
int readRange(int low, int high){
    int number;
    do {
        printf("Please enter a number between 1 and 6: \n");
        if (scanf("%d", &number) != 1) {
            while (getchar() != '\n');
            printf("Please insert a number!\n");
        }
        else if (number < low) printf("Input too low!\n");
        else if (number > high) printf("Input too high!\n");
    } while (number < low || number > high);
    return number;
}
