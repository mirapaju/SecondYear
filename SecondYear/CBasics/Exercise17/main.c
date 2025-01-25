#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

bool password_generator(char *password, int size, const char *word);

#define WORDSIZE 32
#define PASSWORDSIZE (2 * WORDSIZE + 1)

void clear_buffer();

int main() {
    bool program_running = true;
    char prompt[] = "Enter a string of max 31 characters or type \"stop\" to stop:";
    char word[WORDSIZE];
    int size = PASSWORDSIZE;
    int buffer =250;
    char password[size];

    srand(time(NULL));

    while (program_running) {
        printf("%s", prompt);

        if (fgets(word, buffer, stdin) != NULL) {
            size_t len = strlen(word);

            if (len > 0 && word[len - 1] == '\n') {
                word[len - 1] = '\0';
            }
            if (strcmp(word, "stop") == 0) {
                program_running = false;
            }
            else {
                bool result = password_generator(password, size, word);
                if (result) {
                    printf("Generated password: %s\n", password);
                }
                else {
                    printf("Failed to generate password!\n");
                    clear_buffer();
                }
            }
        } else {
            printf("Reading input failed!");
        }
    }
    printf("Byebye!\n");
    return 0;
}

bool password_generator(char *password, int size, const char *word) {
    int word_index=0;
    int password_index=0;

    int max_len = (size - 3) / 2;
    if (strlen(word) > max_len) {
        return false;
    }

    password[password_index] = rand() % (126 - 33) + 33;
    password_index++;
    while (word[word_index] != '\0') {
        password[password_index++] = word[word_index++];
        if (word[word_index] == '\0') {
            password[password_index++] = rand() % (126 - 33) + 33;
        } else {
            password[password_index++] = rand() % (126 - 33) + 33;
        }
    }
    password[password_index] = '\0';

    return true;
}

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}