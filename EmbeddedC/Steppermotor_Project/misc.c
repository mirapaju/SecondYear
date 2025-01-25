#include "pill_dispenser.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/util/queue.h"
#include <stdio.h>

#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define SAVE_INTERVAL 1000000

void setup(){
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    i2c_init(i2c1, 400000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);

    gpio_init(PIEZO_PIN); //initialize sensor
    gpio_set_dir(PIEZO_PIN, GPIO_IN);
    gpio_pull_up(PIEZO_PIN);

    gpio_init(OPTO_FORK_PIN);
    gpio_set_dir(OPTO_FORK_PIN, GPIO_IN);
    gpio_pull_up(OPTO_FORK_PIN);

    gpio_init(LED_PIN); //initialize led
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN); //initialize button
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_set_pulls(BUTTON_PIN, true, false);  // Enable pull-up resistor
}


uint64_t update_time_stamp(Dispenser *dispenser){
    static uint64_t timestamp_of_last_update = 0; //static variable to hold the time of the last update
    uint64_t current_time = time_us_64(); //time passed since powered on

    uint64_t time_since_last_update = current_time - timestamp_of_last_update;
    dispenser->timestamp = dispenser->timestamp + time_since_last_update;
    timestamp_of_last_update = current_time;
    return dispenser->timestamp;
}


void wait_dispensing_interval(Dispenser *dispenser) {
    uint64_t last_save_time=0;
    uint64_t time_elapsed;
    uint64_t current_time;
    do {
        time_elapsed = update_time_stamp(dispenser);
        current_time = time_us_64();
        if (current_time-last_save_time >= SAVE_INTERVAL) { //save the updated timestamp to eeprom once a second to decrease time loss in case of reset while being here
            update_state_to_eeprom(dispenser, DEVICE_STATE_ADDR, DISPENSER_STATE_FORMAT);
            last_save_time = current_time;
            //printf("Updated timestamp: %llu\n", dispenser->timestamp);
        }
        sleep_us(10000);
    } while (time_elapsed <= DISPENSE_INTERVAL);
}

uint16_t crc16(const uint8_t *data, size_t length){
    uint8_t x;
    uint16_t crc = 0xFFFF;
    while (length--) {
        x = crc >> 8 ^ *data++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }
    return crc;
}

uint16_t bytes_to_uint16(uint8_t high_byte, uint8_t low_byte) {
    return ((uint16_t)high_byte << 8) | low_byte;
}

//function to control the LED state
void control_led(bool on) {
    gpio_put(LED_PIN, on);
}

bool is_button_pressed() {
    return !gpio_get(BUTTON_PIN);
}

