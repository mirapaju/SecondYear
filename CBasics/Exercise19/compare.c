#include <string.h>
#include "menu.h"

int comp_price(const void *a, const void *b){
    const menu_item *first = (const menu_item*)a;
    const menu_item *second = (const menu_item*)b;
    if (first->price > second->price){
        return 1;
    }
    else if(first->price < second->price){
        return -1;
    }
    else{
        return 0;
    }
}

int comp_name(const void *a, const void *b){
    const menu_item *first = (const menu_item*)a;
    const menu_item *second = (const menu_item*)b;
    return strcmp(first->name, second->name);
}
