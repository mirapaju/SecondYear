#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MENUSIZE 40
#define ITEMSIZE 50

typedef struct menu_item_ {
    char name[ITEMSIZE];
    double price;
} menu_item;

void init_menu(menu_item *menu, int size);
void read_filename(char *file, char *prompt);
void copy_to_menu(FILE *file, menu_item *menu);
void print_menu(menu_item *menu, int size);
//menu.txt

int main() {
    menu_item menu[MENUSIZE];
    char prompt[] = "Enter a filename: ";
    char file_input[80];
    FILE *file;

    init_menu(menu, MENUSIZE);
    read_filename(file_input, prompt);
    file = fopen(file_input, "r");

    if (file == NULL) { // file was not opened
        fprintf(stderr, "Error opening the file %s!\n", file_input);
        return 1;
    }
    else { //file was successfully opened
        copy_to_menu(file, menu);
    }
    fclose(file);
    print_menu(menu, MENUSIZE);
    return 0;
}

//initialize menu struct with zeroes
void init_menu(menu_item *menu, int size){
    for (int i = 0; i < size; i++) {
        menu[i].name[0] = '\0';
        menu[i].price = 0;
    }
}

void read_filename(char *file, char *prompt) {
    do {
        printf("%s\n", prompt);
        fgets(file, 80, stdin);
    } while (strcmp(file, "\n") == 0);
    if (file[strlen(file) - 1] == '\n') {
        file[strlen(file) - 1] = '\0';
    }
}

//copy the data from file to struct
void copy_to_menu(FILE *file, menu_item *menu) {
    int count = 0;
    char line[256];
    while (count < MENUSIZE && fgets(line, sizeof(line), file) != NULL) {
        char *name_end = strchr(line, ';');
        if (name_end != NULL) {
            size_t nameSize = name_end - line;
            strncpy(menu[count].name, line, nameSize);
            menu[count].name[nameSize] = '\0';
            ++name_end;
            double price = strtod(name_end, NULL);
            menu[count].price = price;
        }
        ++count;
    }
}

void print_menu(menu_item *menu, int size){
    printf("%-8s%s\n", "Price", "Product");
    for (int i = 0; i < size; i++) {
        if ((menu[i].price != 0) && (menu[i].name[0] != '\0')) {
            printf("%-8.2lf%s\n", menu[i].price, menu[i].name);
        }
    }
}
