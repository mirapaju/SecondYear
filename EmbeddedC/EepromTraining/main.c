#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/time.h"
#include "led_states_storing.h"
#include "pico/stdio_uart.h"

#define DEVADDR 0x50
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define LED_STATE_ADDRESS 0xfffe
#define DEFAULT_LED_STATE 0x02



void print_binary(uint8_t hex_value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (hex_value >> i) & 1);
    }
    printf("\n");
}

void write_eeprom_data(uint16_t address, ledstate *ls, int size) {
    uint8_t data_to_write[4] = {(uint8_t)(address >> 8), (uint8_t)address, ls->state, ls->not_state};
    int result = i2c_write_blocking(i2c1, DEVADDR, data_to_write, 4, false);
    sleep_ms(10); 
}

uint8_t read_eeprom_data(uint16_t address, ledstate *ls) {
    uint8_t mem_address[2] = {(uint8_t)(address >> 8), (uint8_t)address};
    size_t mem_size = sizeof(mem_address) / sizeof(mem_address[0]);
    uint8_t data[2] = {1,1};
    size_t data_size = sizeof(data) / sizeof(data[0]);

    i2c_write_blocking(i2c1, DEVADDR, mem_address, mem_size, true);
    i2c_read_blocking(i2c1, DEVADDR, data, data_size, false);
    printf("Eeeprom data after reading, state:0x%02x\n", data[0]);
    if (data[0] != (uint8_t)~data[1]){
        printf("Invalid LED state. \n");
        return -1;
    }
    set_led_state(ls, data[0]);
    return data[0];
}

int main() {
    stdio_uart_init();
    stdio_init_all();

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // Initialize I2C
    i2c_init(i2c1, 400000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);

    uint leds[] = {20,21,22};
    uint buttons[] = {7,8,9};
    size_t size = sizeof(buttons) / sizeof(buttons[0]);

    for (int i=0; i<size; i++){
        init_LED(leds[i]);
        init_button(buttons[i]);
    }

    //store the start time
    uint64_t start_time_us = time_us_64();
    ledstate current_state;
    uint pressed_button;
    uint8_t invalid_state = -1;
    uint8_t led_states = read_eeprom_data(LED_STATE_ADDRESS, &current_state);

    if (led_states == invalid_state) led_states = DEFAULT_LED_STATE;
    update_leds(led_states);
    print_state_and_seconds(start_time_us, led_states);

    while (true) {
        if (button_pushed(buttons, size, &pressed_button)) {
            led_states ^= 1 << (9 - pressed_button);
            printf("LEDSTATES AFTER PUSH: %x\n", led_states);
            update_leds(led_states);
            print_state_and_seconds(start_time_us, led_states);
            set_led_state(&current_state, led_states);
            write_eeprom_data(LED_STATE_ADDRESS, &current_state, 2); // write new LED state to EEPROM
            int value = read_eeprom_data(LED_STATE_ADDRESS, &current_state); //
           // printf("Eeprom value: %d\n", value);
        }
    }
    return 0;
}
