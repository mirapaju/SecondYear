#include "led_states_storing.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/time.h"


void set_led_state(ledstate *ls, uint8_t value) {
    ls->state = value;
    ls->not_state = ~value;
}

bool led_state_is_valid(ledstate *ls) {
    return ls->state == (uint8_t)~ls->not_state;
}

void init_button(uint8_t pin_nro){
    gpio_init(pin_nro);
    gpio_set_dir(pin_nro, GPIO_IN);
    gpio_pull_up(pin_nro);
}

void init_LED(uint pin_nro){
    gpio_init(pin_nro);
    gpio_set_dir(pin_nro, GPIO_OUT);
}

bool button_pushed(const uint *pins, size_t num_pins, uint *pressed_button) {
    for (size_t i = 0; i < num_pins; i++) {
        if (!gpio_get(pins[i])) {
            sleep_ms(500);
            if (gpio_get(pins[i])) {  // Debounce delay
                // Update the pressed button value
                *pressed_button = pins[i];
                return true;
            }
        }
    }
    return false;  // No button pressed
}

void print_state_and_seconds(uint64_t start_time_us, uint8_t led_states) {
    // Print the seconds since the program has started
    uint64_t current_time = time_us_64();
    uint32_t seconds_since_start = (uint32_t)((current_time - start_time_us) / 1000000);
    fprintf(stdout, "Seconds since start: %u. LED State: 0x%02x\n", seconds_since_start, led_states);
}

void update_leds(uint8_t led_states) {
    gpio_put(20, led_states & 0x01);
    gpio_put(21, led_states & 0x02);
    gpio_put(22, led_states & 0x04);
}

