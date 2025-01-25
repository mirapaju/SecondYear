#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void remove_newline(char *str);
int count_words(const char* string, const char *word);
void read_input(char *str, char *modifiers, char *prompt1, char *prompt2);

//asks the user to enter a string, then a word to look for in a string
//if string is "stop" -> end the program
int main() {
    char str[80];
    char word[80];
    int count;
    bool runProgram = true;

    while (runProgram){
        printf("Enter a string, type \"stop\" to stop:\n");
        fgets(str, 80, stdin);
        remove_newline(str);
        if (strcmp(str, "stop") == 0){
            runProgram = false;
        }
        else{
            printf("Enter a word:\n");
            fgets(word, 80, stdin);
            remove_newline(word);
            count = count_words(str, word);
            printf("Words found: %d\n", count);
        }
    } printf("Byebye!");
    return 0;
}

void read_input(char *str, char *modifiers, char *prompt1, char *prompt2) {
    do{
        printf("Please enter a string to be modified: \n");
        fgets(str, 80, stdin);
    } while (strcmp(str, "\n") == 0);
    do {
        printf("Enter a character you want to replace and the character which it will be replaced with \n");
        printf("(for example \"a3\" will replace all the occurrences of letter \"a\" with the letter \"e\") : \n");
        fgets(modifiers, 20, stdin);
    } while(strlen(modifiers) != 3);
}

void remove_newline(char *str) {
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

//returns the number of occurrences of the given word in the given string
int count_words(const char* string, const char *word){
    int count = 0;
    unsigned word_length = strlen(word);

    while ((string = strstr(string, word)) != NULL) {
        string = string + word_length;
        count += 1;
    }
    return count;
}
