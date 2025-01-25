#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 15

void print_numbers(const int *array, int size);

//function fills an array with random numbers
int main() {
    int randomNumbers[SIZE];
    srand(time(NULL));

    for (int i=0; i<SIZE; i++){
        randomNumbers[i] = rand();
    }
    print_numbers(randomNumbers, SIZE);
    return 0;
}

// function prints the numbers of the given array on separate lines in eight characters wide field
void print_numbers(const int *array, int size){
    for (int i=0; i<size; i++){
        printf("%8d\n", array[i]);
    }
}


