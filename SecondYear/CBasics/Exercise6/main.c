#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void printMenu(void);
int operationSelection();
void randomNumber(int maxOutcome);

//function for running the dice game as long as the user input is 3
int main() {
    int selection;
    int sixSidedDie = 6;
    int tenSidedDie = 10;
    bool roll= true;

    printf("Welcome to the dice roller!\n");

    while(roll){
        printMenu();
        selection = operationSelection();
        switch (selection){
            case 1:
                randomNumber(sixSidedDie);
                break;
            case 2:
                randomNumber(tenSidedDie);
                break;
            case 3:
                roll = false;
            default:
                break;
        }
    }
    printf("Thanks for playing!\n");
    return 0;
}

//function for printing the possible operations
void printMenu(void){
    printf("\nDo you want to:\n");
    printf("1) Roll D6\n");
    printf("2) Roll D10\n");
    printf("3) Quit\n");
}

//function for choosing the operation+validating the user input
int operationSelection(){
    int number;
    bool invalidInput = true;
    while (invalidInput){
        printf("Select operation: \n");
        if (scanf("%d", &number) != 1) {
            while (getchar() != '\n');
            printf("Please enter a number not a letter!\n");
            printMenu();
        }
        else if (number> 3 || number < 1){
            printf("Input out of range!\n");
            printMenu();
        }
        else{
            invalidInput = false;
        }
    }
    return number;
}

//function to generate a random number between the desired range (1-6 or 1-10 this case)
void randomNumber(int maxOutcome){
    srand(time(NULL));
    int minOutcome = 1;
    int dieResult = rand() % maxOutcome + minOutcome;
    printf("You got %d with the %d-sided die! \n", dieResult, maxOutcome);
}

