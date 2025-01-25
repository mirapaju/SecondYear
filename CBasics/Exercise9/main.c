#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 20

void initArray(unsigned int *array, int size);
void printArray(unsigned int *array, int size);
void read_number(int *choice, const char *prompt);
int find_first(const unsigned int *array, unsigned int what);

int main() {
    unsigned int randomNumbers[SIZE];
    bool runProgram = true;
    int choice;
    int index;

    initArray(randomNumbers, SIZE);
    printArray(randomNumbers, SIZE);

    while (runProgram){
        read_number(&choice, "Choose a number between 1 and 20 to search, or enter 0 to stop: \n");
        if (choice != 0){
            index = find_first(randomNumbers, choice);
            if (index == -1){
                printf("Number not found.\n");
            }
            else{
                printf("Number found at the index: %d\n", index);
            }
        }
        else{
            runProgram = false;
        }
    }
    printf("Byebye!");
    return 0;
}

//function for initializing the array with random numbers, last number=0
void initArray(unsigned int array[], int size){
    int i;
    srand(time(NULL));
    for (i=0; i<size-1; i++) {
        array[i] = (rand() % size) +1;
    }
    array[i]= '\0';
}

void printArray(unsigned int array[], int size){
    for (int i=0; i<size; i++){
        printf("%d\n", array[i]);
    }
}

//function for reading the user input and validating it
void read_number(int *choice, const char *prompt){
    int number;
    char input[32];
    bool invalidInput = true;

    while (invalidInput){
        printf("%s", prompt);
        fgets(input, 32, stdin);

        if (sscanf(input,"%d", &number) != 1) {
            printf("Incorrect input!\n");
        }
        else if ((number < 0) || (number > 20)){
            printf("Number out of range!\n");
        }
        else {
            *choice = number;
            invalidInput = false;
        }
    }
}

// searches for the given number in the array, returns the index. not found -> returns -1
int find_first(const unsigned int *array, unsigned int what){
    for (int i=0; array[i] != '\0' ; i++) {
        if (array[i] == what) {
            return i + 1;
        }
    } return -1;
}

