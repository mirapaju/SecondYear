#include <stdio.h>
#include <string.h>

#define LINESIZE 300
#define FILENAMESIZE 80
void read_filename(char *file, char *prompt);

//The result of bitwise XOR operator is 1 if the
// corresponding bits of two operands are opposite, denoted by ^
int calculate_checksum(char *lineptr);

int main() {
    char file_input[FILENAMESIZE];
    char prompt[] ="Enter a filename: ";
    FILE *file;
    char line[LINESIZE];
    int calculated_checksum;
    int checksum;

    read_filename(file_input, prompt);
    file = fopen(file_input, "r");
    if (file==NULL){
        fprintf(stderr, "Error opening the file %s!\n", file_input);
        return 1;
    }
    else{
        while (fgets(line, sizeof(line), file) != NULL){
            if (line[0] == '$' && strchr(line, '*')){
                calculated_checksum = calculate_checksum(line);
                sscanf(strchr(line, '*') + 1, "%2x", &checksum);
                if (checksum == calculated_checksum){
                    printf("[ OK ] %s\n", line);
                }
                else {
                    printf("[FAIL] %s\n", line);
                }

            }
        }
    }
    fclose(file);
    return 0;
}

void read_filename(char *file, char *prompt) {
    do {
        printf("%s\n", prompt);
        fgets(file, FILENAMESIZE, stdin);
    } while (strcmp(file, "\n") == 0);
    if (file[strlen(file) - 1] == '\n') {
        file[strlen(file) - 1] = '\0';
    }
}

int calculate_checksum(char *lineptr){
    int checksum=0;
    for (int i = 1; lineptr[i] != '*'; i++) {
        checksum = checksum ^ lineptr[i];
    }
    return checksum;
}


