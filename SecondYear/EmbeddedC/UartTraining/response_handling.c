#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "uart.h"
#include "ctype.h"

bool validate_response(int uart_nr, char *str, int size, char *expected_str) {
    int pos = uart_read(uart_nr, (uint8_t *) str, size);
    printf("Validating...\n");
    printf("Characters received: %d\n", pos);
    if (pos >0) {
        str[pos] = '\0';
        if (strstr(str, expected_str) !=NULL){
            printf("Data is not valid!\n");
            return false;
        }
        else {
            printf("Strings match.\n");
            return true;
        }
    }
    else{
        printf("No response in buffer, could not validate.");
        return false;
    }
}


void modify_devEui_response(const char *devEui, char *modified_DevEui){
    int j = 0;

    for (int i = 13; devEui[i] != '\0'; i++) {
        if (isxdigit(devEui[i])) {
            modified_DevEui[j++] = tolower(devEui[i]);
        }
    }
    modified_DevEui[j] = '\0';
}

void get_uart_strings(uart_inst_t uart_nr, char *str, int size){
    printf("Retrieving response...\n");
    int pos =0;

    while (true){
    if (uart_is_readable(uart_nr)) {
        char character = uart_getc(uart_nr);
        str[pos++] = character;
        printf("%c", character);
        if (character == '\n' || pos >= size - 1) {
            str[pos] = '\0';
        }
    }
    }
}


