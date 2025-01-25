#include <iostream>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"
#include <queue>

#define NUM_LEDS 3
#define LED_MAX_STATES 7
#define LED_1 20
#define LED_2 21
#define LED_3 22

//set the toggling interval according to the led you want toggle the fastest
//20 -> 125ms -> 4/sec
//21 -> 250ms -> 2/sec
//22 -> 500ms -> 1/sec
#define INTERVAL 125

//set the printing interval according to how often you want to print out the time elapsed
#define PRINT_INTERVAL 1000
#define US_PER_S (1000000)

std::queue<int> events;

void print_counter_bits(uint8_t mask) {
    printf("Mask bits: ");
    for (int i = 7; i >= 0; i--) printf("%d", (mask >> i) & 1);
    printf("\n");
}

void print_elapsed_time(absolute_time_t start_time){
    absolute_time_t current_time = get_absolute_time();
    uint32_t elapsed_seconds = absolute_time_diff_us(start_time, current_time) / US_PER_S;
    printf("Elapsed time: %d seconds\n", elapsed_seconds);
}

bool tmr_cb(struct repeating_timer *t){
    events.push(1);
    return true;
}

int main() {
    stdio_init_all();
    printf("Boot\n");

    absolute_time_t start_time = get_absolute_time();
    struct repeating_timer tmr;
    if (add_repeating_timer_ms(PRINT_INTERVAL, tmr_cb, NULL, &tmr)) printf("Timer added\n");

    const uint leds[NUM_LEDS] = {LED_1, LED_2, LED_3};

    for (auto pin : leds) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
    }

    int counter;
    int led;
    while (true) {
        if (!events.empty()) print_elapsed_time(start_time);
        for (counter = 0; counter < LED_MAX_STATES; counter++) {
            for (led = 0; led < NUM_LEDS; led++) {
                gpio_put(leds[led], (counter >> led) & 1);
            }
            sleep_ms(INTERVAL);
        }
    }
    return 0;
}
