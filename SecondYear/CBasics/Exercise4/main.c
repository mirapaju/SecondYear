#include <stdio.h>

int readInteger(void);
float calculateAverage(int sum, int count);

//asks for positive numbers until input is negative
int main() {
    int number;
    int count=0;
    int sum=0;
    do {
        printf("Enter positive numbers or negative to stop: \n");
        number = readInteger();
        if (number >0){
            count+=1;
            sum += number;
        }
    } while (number >= 0);
    //float average = (float)sum / (float)count;
    float average = calculateAverage(sum, count);
    printf("You entered %d positive numbers. The average is: %.3f", count, average);
    return 0;
}

//function for returning one user input at a time
int readInteger(void){
    int number;
    if (scanf("%d", &number) != 1) {
        while(getchar() != '\n');
        printf("The input can't contain letters.\n");
        return 0;
    } else if (number==0){
        printf("Zero is nor positive or negative number!\n");
    }
    return number;
}

float calculateAverage(int sum, int count){
    if (count > 0){
        float av=(float)sum / (float)count;
        return av;
    }
    else return 0;
}
