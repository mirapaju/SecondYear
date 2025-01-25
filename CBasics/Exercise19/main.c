#include <stdio.h>
#include <stdlib.h>
#include "menu.h"


int main() {
    menu_item menu[MENUSIZE];
    char prompt[] = "Enter a filename: ";
    char sort_prompt[] = "Do you want to sort menu by:\n1.Name\n2.Price\n";
    char file_input[80];
    FILE *file;
    int choice;

    init_menu(menu, MENUSIZE);
    read_filename(file_input, prompt);
    file = fopen(file_input, "r");

    if (file == NULL) { // file was not opened
        fprintf(stderr, "Error opening the file %s!\n", file_input);
        return 1;
    }
    else { //file was successfully opened
        copy_to_menu(file, menu);
        print_menu(menu, MENUSIZE);

        choice = read_range(sort_prompt, 1, 2);
        if (choice==2){
            size_t menu_size = sizeof(menu) / sizeof(menu[0]);
            qsort(menu, menu_size, sizeof(menu_item), comp_price);
            print_menu(menu, MENUSIZE);
        } else if (choice==1){
            size_t menu_size = sizeof(menu) / sizeof(menu[0]);
            qsort(menu, menu_size, sizeof(menu_item), comp_name);
            print_menu(menu, MENUSIZE);
        } else{
            printf("Something went wrong.");
        }
    }
    fclose(file);
    return 0;
}


