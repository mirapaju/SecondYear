#ifndef EXERCISE19_MENU_H
#define EXERCISE19_MENU_H
#include <stdio.h>


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
int read_range(const char *sort_prompt, int min, int max);
int comp_price(const void *a, const void *b);
int comp_name(const void *a, const void *b);

#endif //EXERCISE19_MENU_H
