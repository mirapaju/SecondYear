#include <stdio.h>
#include <stdbool.h>

bool readPositive(int *value, const char *prompt);

//Asks the user to guess the money amount
//Calculates the money amount to be +20 of what the user guesses
//program stops when user has entered invalid input 3 times
int main() {
    int moneyGuess = 0;
    int maxInvalidInputs = 3;
    int invalidInputCount = 0;
    bool validInput = false;

    while (invalidInputCount < maxInvalidInputs) {
        validInput = readPositive(&moneyGuess, "Guess how much money I have!\nEnter a positive number: \n");
        (!validInput) ? ++invalidInputCount :
                        printf("You didn't get it right. I have %d euros.\n", moneyGuess + 20);
    }
    printf("I give up! See you later!");
    return 0;
}

//function for reading the user input, returns true if valid
bool readPositive(int *moneyGuess, const char *prompt){
    int number = 0;
    char input[32];
    printf("%s", prompt);

    fgets(input, 32, stdin);
    if ((sscanf(input, "%d", &number) != 1) | (number < 0)) {
        printf("Incorrect input!\n");
        return false;
    }
    else{
        *moneyGuess = number;
        return true;
    }
}


