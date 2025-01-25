#include <iostream>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"

void toggle_leds(uint8_t mask) { //initial mask is 0000 0000
    gpio_put(led_1, mask & 0b001); // first bit (LSB) of the mask
    gpio_put(led_2, mask & 0b010); // second bit of the mask
    gpio_put(led_3, mask & 0b100); // third bit (MSB) of the mask
}

int main() {
    stdio_init_all();
    absolute_time_t start_time = get_absolute_time();
    const int led_1 = 19;
    const int led_2 = 20;
    const int led_3 = 21;
    const int leds[] = {led_1, led_2, led_3};

    for (auto led : leds){
        gpio_init(led);
        gpio_set_dir(led, GPIO_OUT);
    }

    uint8_t mask =0;
    while (true){
        absolute_time_t current_time = get_absolute_time();
        uint32_t elapsed_seconds = absolute_time_diff_us(current_time, get_absolute_time()) / 1'000'000;

        mask = ((elapsed_seconds / 4) % 2) << 2;  // LED D1 blinks 4 times per second
        mask |= ((elapsed_seconds / 2) % 2) << 1; // LED D2 blinks 2 times per second
        mask |= (elapsed_seconds % 2);            // LED D3 blinks 1 time per second
    }
    return 0;
}

