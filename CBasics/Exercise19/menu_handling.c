#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"

//initialize menu struct with zeroes
void init_menu(menu_item *menu, int size){
    for (int i = 0; i < size; i++) {
        menu[i].name[0] = '\0';
        menu[i].price = 0;
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
            strncpy(menu[count].name, line, ITEMSIZE-1);
            menu[count].name[nameSize] = '\0';
            ++name_end;
            double price = strtod(name_end, NULL);
            if (price != 0.0) {
                menu[count].price = price;
            }
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
