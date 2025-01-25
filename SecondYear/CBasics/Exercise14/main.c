#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINESIZE 80
#define LINECOUNT 100

void get_filename(char *filePath, char *prompt);

//convert contents of a file to uppercase and save to file
int main() {
    char filePath[LINESIZE];
    char prompt[] = ("Enter filename: \n");
    char array[LINECOUNT][LINESIZE];
    int count = 0;
    FILE *file;

    get_filename(filePath, prompt);
    file = fopen(filePath, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening the file %s!\n", filePath);
        return 1;
    }
    else { //file opened
        while (count < LINECOUNT && !feof(file)) {
            if (fgets(array[count], LINESIZE, file) != NULL) { //if reading to array is ok
                count++;
            } else {
                    fprintf(stderr, "Error reading from file %s.\n", filePath);
                    return 1;
            }
        }
        fclose(file);
        for (int i=0; i<count; i++){ //convert everything to uppercase in the array
            for (int j=0; j<LINESIZE; j++){
                array[i][j]= toupper(array[i][j]);
            }
        }
        if (fopen(filePath, "w") == NULL) {
            fprintf(stderr, "Error opening the file %s in reading mode!\n", filePath);
            return 1;
        }
        else{ //opened in writing mode ok
            for (int i=0; i<count; i++){
                if (fputs(array[i], file) == EOF) { //fputs returns EOF in case of an error
                    fprintf(stderr, "Error writing to file %s!\n", filePath);
                    return 1;
                }
            }
        }
    }
    fclose(file);
    return 0;
}

void get_filename(char *filePath, char *prompt) {
    do {
        printf("%s\n", prompt);
        fgets(filePath, 150, stdin);
    } while (strcmp(filePath, "\n") == 0);
    if (filePath[strlen(filePath) - 1] == '\n') {
        filePath[strlen(filePath) - 1] = '\0';
    }
}
