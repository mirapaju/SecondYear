#include <stdio.h>
#include <string.h>

void read_input(char *str, char *modifiers);
void remove_newline(char *str);
int replace_char(char *str, const char *repl);

int main() {
    int charactersFound;
    char text[80];
    char modifiers[20];

    read_input(text, modifiers);
    remove_newline(text);
    charactersFound = replace_char(text, modifiers);

    if (charactersFound == 0){
        printf("No occurrences found, the string was not modified.\n");
    }
    else {
        printf("%d characters were replaced.\n", charactersFound);
        printf("The modified string: %s.\n", text);
    }
    return 0;
}

void read_input(char *str, char *modifiers) {
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

//replaces the given character with the second given character in the string
//replace length should be validated
int replace_char(char *str, const char *repl) {
    int count=0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == repl[0]) {
            str[i] = repl[1];
            count += 1;
        }
    } return count;
}

