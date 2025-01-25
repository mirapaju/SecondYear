#include <stdio.h>
#include <string.h>

void read_input(char *filePath, char *prompt);
int find_extreme_values(FILE *file, int *biggest, int *lowest);

int main() {
    int biggest=0;
    int lowest =0;
    int count;
    char prompt[] = "Enter a file to open: ";
    char filePath[80];
    FILE *file;

    read_input(filePath, prompt);
    file = fopen(filePath, "r");
    if (file == NULL) { // file was not opened
        fprintf(stderr, "Error opening the file %s!\n", filePath);
        return 1;
    }
    else { //file was successfully opened
        count = find_extreme_values(file, &biggest, &lowest);
    }
    fclose(file);
    printf("Total of numbers read: %d. Biggest number: %d, lowest number: %d.\n", count, biggest, lowest);
    return 0;
}

void read_input(char *filePath, char *prompt) {
    do {
        printf("%s\n", prompt);
        fgets(filePath, 80, stdin);
    } while (strcmp(filePath, "\n") == 0);
    if (filePath[strlen(filePath) - 1] == '\n') {
        filePath[strlen(filePath) - 1] = '\0';
    }
}

//searches the biggest and lowest value of the integers in the file
int find_extreme_values(FILE *file, int *biggest, int *lowest) {
    int num = 0;
    int count = 0;
    if (fscanf(file, "%d", &num) == 1) {
        *biggest = num;
        *lowest = num;
        count++;
    } else{
        printf(stderr, "Unable to read first integer.");
    }
    while (fscanf(file, "%d", &num) == 1) {
        count++;
        if (num > *biggest) {
            *biggest = num;
        } else if (num < *lowest) {
            *lowest = num;
        }
    }
    return count;
}




